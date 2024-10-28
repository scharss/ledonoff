#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Configuración del LED (puedes cambiar el pin si es necesario)
const int LED_PIN = 2;  // GPIO 2 (D4 en NodeMCU)

// Credenciales de la red WiFi
const char* ssid = "xxxxxx";  // Reemplaza con el nombre de tu red WiFi
const char* password = "xxxxx";  // Reemplaza con la contraseña de tu red WiFi

ESP8266WebServer server(80);  // Inicialización del servidor web en el puerto 80

String ledState = "apagado";  // Variable para almacenar el estado del LED

void setup() {
  // Inicializa el LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Apaga el LED al iniciar (el LED está apagado en estado LOW)

  // Inicializa el puerto serie para depuración
  Serial.begin(115200);
  delay(10);
  
  // Conexión a la red WiFi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configura las rutas del servidor web
  server.on("/", handleRoot);  // Ruta para la página principal
  server.on("/LED=ON", handleLEDOn);  // Ruta para encender el LED
  server.on("/LED=OFF", handleLEDOff);  // Ruta para apagar el LED

  server.begin();  // Inicia el servidor
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();  // Atiende las solicitudes de los clientes
}

// Función para manejar la página principal
void handleRoot() {
  String html = "<html>\
                 <head>\
                   <title>Control del LED</title>\
                   <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
                   <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css\">\
                 </head>\
                 <body class='text-center'>\
                   <h1>Control del LED</h1>\
                   <button class='btn btn-success' onclick=\"sendRequest('/LED=ON')\">Encender LED</button>\
                   <button class='btn btn-danger' onclick=\"sendRequest('/LED=OFF')\">Apagar LED</button>\
                   <p id='status'>Estado del LED: " + ledState + "</p>\
                   <script>\
                     function sendRequest(url) {\
                       fetch(url)\
                       .then(response => response.text())\
                       .then(data => {\
                         document.getElementById('status').innerHTML = data;\
                       });\
                     }\
                   </script>\
                 </body>\
                 </html>";
  server.send(200, "text/html", html);
}

// Función para encender el LED
void handleLEDOn() {
  digitalWrite(LED_PIN, HIGH);  // Enciende el LED
  ledState = "encendido";
  server.send(200, "text/html", "LED encendido");
  Serial.println("LED Encendido");
}

// Función para apagar el LED
void handleLEDOff() {
  digitalWrite(LED_PIN, LOW);  // Apaga el LED
  ledState = "apagado";
  server.send(200, "text/html", "LED apagado");
  Serial.println("LED Apagado");
}
