#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#include "wifi_manager.h"

// ============================================
// CONFIGURACION DEL PORTAL
// ============================================

const char* AP_SSID = "ANDADOR_CONFIG";
const char* AP_PASSWORD = "andador123";

// ============================================
// OBJETOS
// ============================================

WebServer servidor(80);
Preferences preferencias;

bool portalActivo = false;

// ============================================
// GUARDAR CREDENCIALES
// ============================================

void guardarCredenciales(String ssid, String password) {

    preferencias.begin("wifi", false);

    preferencias.putString("ssid", ssid);
    preferencias.putString("password", password);

    preferencias.end();

    Serial.println("Credenciales WiFi guardadas");
}

// ============================================
// LEER CREDENCIALES
// ============================================

bool leerCredenciales(String& ssid, String& password) {

    preferencias.begin("wifi", true);

    ssid = preferencias.getString("ssid", "");
    password = preferencias.getString("password", "");

    preferencias.end();

    return ssid.length() > 0;
}

// ============================================
// GENERAR LISTA DE REDES
// ============================================

String generarOpcionesWiFi() {

    Serial.println("Buscando redes WiFi...");

    int cantidad = WiFi.scanNetworks();

    String opciones = "";

    if (cantidad <= 0) {

        opciones +=
            "<option value=''>No se encontraron redes</option>";

        Serial.println("No se encontraron redes");

        return opciones;
    }

    Serial.print("Redes encontradas: ");
    Serial.println(cantidad);

    for (int i = 0; i < cantidad; i++) {

        String ssid = WiFi.SSID(i);

        int rssi = WiFi.RSSI(i);

        opciones += "<option value='";
        opciones += ssid;
        opciones += "'>";

        opciones += ssid;

        opciones += " (";
        opciones += String(rssi);
        opciones += " dBm)";

        opciones += "</option>";

        Serial.print(i + 1);
        Serial.print(" - ");
        Serial.print(ssid);
        Serial.print(" | ");
        Serial.print(rssi);
        Serial.println(" dBm");
    }

    WiFi.scanDelete();

    return opciones;
}

// ============================================
// PAGINA HTML
// ============================================

String paginaConfiguracion() {

    String redes = generarOpcionesWiFi();

    String pagina = R"rawliteral(
<!DOCTYPE html>

<html lang="es">

<head>

<meta charset="UTF-8">

<meta name="viewport"
content="width=device-width, initial-scale=1.0">

<title>Andador Asistido</title>

<style>

body {

    font-family: Arial, sans-serif;

    background: #f2f2f2;

    text-align: center;

    padding: 25px;
}

.contenedor {

    max-width: 420px;

    margin: auto;

    background: white;

    padding: 25px;

    border-radius: 12px;
}

select,
input {

    width: 95%;

    padding: 12px;

    margin: 8px 0;

    font-size: 16px;
}

button {

    width: 95%;

    padding: 12px;

    margin-top: 15px;

    font-size: 17px;

    cursor: pointer;
}

</style>

</head>

<body>

<div class="contenedor">

<h2>ANDADOR ASISTIDO</h2>

<p>Configuracion de red WiFi</p>

<form action="/conectar" method="POST">

<label>Red disponible:</label>

<br>

<select name="ssid" required>

)rawliteral";

    pagina += redes;

    pagina += R"rawliteral(

</select>

<br>

<div style="position:relative; width:95%; margin:auto;">

<input
type="password"
id="password"
name="password"
placeholder="Contraseña WiFi"
style="width:100%; box-sizing:border-box; padding-right:45px;">

<span
onclick="mostrarPassword()"
style="
position:absolute;
right:12px;
top:50%;
transform:translateY(-50%);
cursor:pointer;
font-size:20px;
">
👁️
</span>

</div>

<br>

<button type="submit">
CONECTAR
</button>

</form>

<br>

<a href="/">
Actualizar redes
</a>

</div>
<script>
function mostrarPassword() {
    const campo = document.getElementById("password");

    if (campo.type === "password") {
        campo.type = "text";
    } else {
        campo.type = "password";
    }
}
</script>

</body>

</html>
)rawliteral";

    return pagina;
}

// ============================================
// PAGINA PRINCIPAL
// ============================================

void manejarPaginaPrincipal() {

    servidor.send(
        200,
        "text/html",
        paginaConfiguracion()
    );
}

// ============================================
// INTENTAR CONEXION
// ============================================

bool conectarARed(
    String ssid,
    String password
) {

    Serial.println();

    Serial.println("==============================");
    Serial.println("INTENTANDO CONEXION WIFI");
    Serial.println("==============================");

    Serial.print("Red: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_AP_STA);

    WiFi.begin(
        ssid.c_str(),
        password.c_str()
    );

    unsigned long inicio = millis();

    const unsigned long tiempoMaximo = 15000;

    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - inicio < tiempoMaximo
    ) {

        delay(500);

        Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {

        Serial.println("WiFi conectado correctamente");

        Serial.print("IP local: ");
        Serial.println(WiFi.localIP());

        Serial.print("RSSI: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");

        guardarCredenciales(
            ssid,
            password
        );

        WiFi.setAutoReconnect(true);

        return true;
    }

    Serial.println(
        "No fue posible conectar a la red"
    );

    return false;
}

// ============================================
// RECIBIR DATOS DEL PORTAL
// ============================================

void manejarConexion() {

    if (!servidor.hasArg("ssid")) {

        servidor.send(
            400,
            "text/plain",
            "SSID no recibido"
        );

        return;
    }

    String ssid =
        servidor.arg("ssid");

    String password =
        servidor.arg("password");

    servidor.send(
        200,
        "text/html",

        "<html>"
        "<body style='font-family:Arial;text-align:center;padding:30px;'>"
        "<h2>Conectando...</h2>"
        "<p>El andador intentara conectarse a la red seleccionada.</p>"
        "</body>"
        "</html>"
    );

    bool conectado =
        conectarARed(
            ssid,
            password
        );

    if (conectado) {

        Serial.println(
            "Cerrando portal de configuracion..."
        );

        servidor.stop();

        WiFi.softAPdisconnect(true);

        portalActivo = false;

        WiFi.mode(WIFI_STA);

        Serial.println(
            "Portal cerrado"
        );

        Serial.println(
            "El sistema continuara ejecutandose normalmente"
        );
    }
}

// ============================================
// INICIAR PORTAL
// ============================================

void iniciarPortalWiFi() {

    Serial.println();

    Serial.println("==============================");
    Serial.println("PORTAL WIFI");
    Serial.println("==============================");

    WiFi.mode(WIFI_AP_STA);

    bool resultado =
        WiFi.softAP(
            AP_SSID,
            AP_PASSWORD
        );

    if (!resultado) {

        Serial.println(
            "ERROR iniciando Access Point"
        );

        return;
    }

    portalActivo = true;

    Serial.print("Red creada: ");
    Serial.println(AP_SSID);

    Serial.print("Clave: ");
    Serial.println(AP_PASSWORD);

    Serial.print("IP portal: ");
    Serial.println(WiFi.softAPIP());

    servidor.on(
        "/",
        HTTP_GET,
        manejarPaginaPrincipal
    );

    servidor.on(
        "/conectar",
        HTTP_POST,
        manejarConexion
    );

    servidor.begin();

    Serial.println(
        "Servidor web iniciado"
    );
}

// ============================================
// CONECTAR CON CREDENCIALES GUARDADAS
// ============================================

bool conectarWiFiGuardado() {

    String ssid;
    String password;

    if (!leerCredenciales(
        ssid,
        password
    )) {

        Serial.println(
            "No existen credenciales WiFi guardadas"
        );

        return false;
    }

    Serial.println();

    Serial.println("==============================");
    Serial.println("WIFI GUARDADO");
    Serial.println("==============================");

    Serial.print("Intentando conectar a: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        ssid.c_str(),
        password.c_str()
    );

    unsigned long inicio =
        millis();

    const unsigned long tiempoMaximo =
        5000;

    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - inicio < tiempoMaximo
    ) {

        delay(500);

        Serial.print(".");
    }

    Serial.println();

    if (
        WiFi.status() == WL_CONNECTED
    ) {

        Serial.println(
            "WiFi conectado"
        );

        Serial.print("IP: ");
        Serial.println(
            WiFi.localIP()
        );

        WiFi.setAutoReconnect(true);

        return true;
    }

    Serial.println(
        "No se pudo conectar al WiFi guardado"
    );

    return false;
}

// ============================================
// INICIO GENERAL
// ============================================

void iniciarWiFi() {

    if (!conectarWiFiGuardado()) {

        iniciarPortalWiFi();
    }
}

// ============================================
// PROCESAR SERVIDOR
// ============================================

void procesarWiFi() {

    if (portalActivo) {

        servidor.handleClient();
    }
}

// ============================================
// CONSULTAS
// ============================================

bool wifiConectado() {

    return
        WiFi.status() ==
        WL_CONNECTED;
}

bool portalWiFiActivo() {

    return portalActivo;
}

// ============================================
// INFORMACION
// ============================================

void imprimirEstadoWiFi() {

    if (wifiConectado()) {

        Serial.print(
            "WiFi OK | IP: "
        );

        Serial.print(
            WiFi.localIP()
        );

        Serial.print(
            " | RSSI: "
        );

        Serial.print(
            WiFi.RSSI()
        );

        Serial.println(
            " dBm"
        );
    }

    else if (portalActivo) {

        Serial.print(
            "Portal WiFi activo | IP: "
        );

        Serial.println(
            WiFi.softAPIP()
        );
    }

    else {

        Serial.println(
            "WiFi desconectado"
        );
    }
}