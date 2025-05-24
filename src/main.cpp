#include <WiFi.h>
#include <WebServer.h>
#include <SD_MMC.h>
#include <FS.h>

const char *ssid = "Phone killer";
const char *password = "victor123";

WebServer server(80);

struct CodeInfo
{
    String phone;
    String code;
    String locker;
};
CodeInfo latestCode;

String generateCode()
{
    String code = "";
    for (int i = 0; i < 6; i++)
    {
        code += String(random(0, 10));
    }
    return code;
}

void handleRoot()
{
    server.send(200, "text/html", "<h1>Login Page</h1>");
}

void handleSignup()
{
    server.send(200, "text/html", "<h1>Signup Page</h1>");
}

void handleAdmin()
{
    File file = SD_MMC.open("/admin.html");
    if (!file || file.isDirectory())
    {
        server.send(404, "text/plain", "admin.html not found");
        return;
    }
    server.streamFile(file, "text/html");
}

void handleSendCode()
{
    if (!server.hasArg("phone") || !server.hasArg("locker"))
    {
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

void handleVerify()
{
    if (!server.hasArg("code"))
    {
        server.send(400, "text/plain", "Missing code");
        return;
    }

    String code = server.arg("code");
    if (code == latestCode.code)
    {
        server.send(200, "text/plain", "valid");
    }
    else
    {
        server.send(403, "text/plain", "invalid");
    }
}

void handleUploadMeta();

void setup()
{
    Serial.begin(115200);
    delay(1000);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());

    if (!SD_MMC.begin())
    {
        Serial.println("SD_MMC Mount Failed");
    }
    else
    {
        Serial.println("SD card ready");
    }

    server.on("/", handleRoot);
    server.on("/signup", handleSignup);
    server.on("/admin", handleAdmin);
    server.on("/send_code", handleSendCode);
    server.on("/verify", handleVerify);
    server.on("/items", HTTP_GET, []()
              {
    File root = SD_MMC.open("/");
    String json = "[";
    File file = root.openNextFile();
    while (file) {
      String fname = String(file.name());
      if (!file.isDirectory() && fname.endsWith(".json")) {
        if (json.length() > 1) json += ",";
        json += "\"" + fname + "\"";
      }
      file = root.openNextFile();
    }
    json += "]";
    server.send(200, "application/json", json); });

    server.on("/item_data", HTTP_GET, []()
              {
    if (!server.hasArg("name")) {
      server.send(400, "text/plain", "Missing name param");
      return;
    }
    String fname = "/" + server.arg("name");
    File file = SD_MMC.open(fname);
    if (!file || file.isDirectory()) {
      server.send(404, "text/plain", "File not found");
      return;
    }
    server.streamFile(file, "application/json"); });
    server.on("/upload", HTTP_POST, []()
              { server.send(200, "text/html", "<h2>Upload success</h2><a href='/admin'>Back to admin</a>"); }, handleUploadMeta);

    server.begin();
    Serial.println("Web server started");
}

void handleUploadMeta()
{
    HTTPUpload &upload = server.upload();
    static String imageName;

    if (upload.status == UPLOAD_FILE_START)
    {
        imageName = String(millis()) + ".jpg";
        File file = SD_MMC.open("/" + imageName, FILE_WRITE);
        file.close();
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        File file = SD_MMC.open("/" + imageName, FILE_APPEND);
        if (file)
        {
            file.write(upload.buf, upload.currentSize);
            file.close();
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        // Save metadata
        String jsonFile = "/" + imageName + ".json";
        File meta = SD_MMC.open(jsonFile, FILE_WRITE);
        if (meta)
        {
            meta.print("{");
            meta.printf("  \"type\": \"%s\",", server.arg("type").c_str());
            meta.printf("  \"colour\": \"%s\",", server.arg("colour").c_str());
            meta.printf("  \"location\": \"%s\",", server.arg("location").c_str());
            meta.printf("  \"symbol\": \"%s\",", server.arg("symbol").c_str());
            meta.printf("  \"date\": \"%s\",", server.arg("date").c_str());
            meta.printf("  \"locker\": \"%s\",", server.arg("locker").c_str());
            meta.printf("  \"phone\": \"%s\",", server.arg("phone").c_str());
            meta.printf("  \"image\": \"%s\"", imageName.c_str());
            meta.print("}");
            meta.close();
        }
    }
}

void loop()
{
    server.handleClient();
}
