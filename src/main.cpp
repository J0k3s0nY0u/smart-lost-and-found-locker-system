#include <WiFi.h>
#include <WebServer.h>
#include <SD_MMC.h>
#include <FS.h>

const char* ssid = "Phone killer";
const char* password = "victor123";

WebServer server(80);

struct CodeInfo {
  String phone;
  String code;
  String locker;
};
CodeInfo latestCode;

String generateCode() {
  String code = "";
  for (int i = 0; i < 6; i++) {
    code += String(random(0, 10));
  }
  return code;
}

void handleRoot() {
  server.send(200, "text/html", "<h1>Login Page</h1>");
}

void handleSignup() {
  server.send(200, "text/html", "<h1>Signup Page</h1>");
}

void handleAdmin() {
  File root = SD_MMC.open("/");
  String html = "<h1>Admin Dashboard</h1><ul>";
  File file = root.openNextFile();
  while (file) {
    html += "<li>" + String(file.name()) + "</li>";
    file = root.openNextFile();
  }
  html += "</ul>";
  server.send(200, "text/html", html);
}

void handleSendCode() {
  if (!server.hasArg("phone") || !server.hasArg("locker")) {
    server.send(400, "application/json", "{\"error\":\"Missing phone or locker\"}");
    return;
  }

  latestCode.phone = server.arg("phone");
  latestCode.locker = server.arg("locker");
  latestCode.code = generateCode();

  String json = "{";
  json += "\"phone\":\"" + latestCode.phone + "\",";
  json += "\"code\":\"" + latestCode.code + "\",";
  json += "\"locker\":\"" + latestCode.locker + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleVerify() {
  if (!server.hasArg("code")) {
    server.send(400, "text/plain", "Missing code");
    return;
  }

  String code = server.arg("code");
  if (code == latestCode.code) {
    server.send(200, "text/plain", "valid");
  } else {
    server.send(403, "text/plain", "invalid");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  if (!SD_MMC.begin()) {
    Serial.println("SD_MMC Mount Failed");
  } else {
    Serial.println("SD card ready");
  }

  server.on("/", handleRoot);
  server.on("/signup", handleSignup);
  server.on("/admin", handleAdmin);
  server.on("/send_code", handleSendCode);
  server.on("/verify", handleVerify);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
