#include <ESP8266WiFi.h>

// --- TES IDENTIFIANTS WIFI ---
const char* ssid     = "iPhone";      
const char* password = "ndeya226";    

WiFiServer server(80);

// --- VARIABLES POUR LE GRAPHIQUE ---
const int TAILLE_HISTORIQUE = 10;          
int historique[TAILLE_HISTORIQUE] = {0};   
unsigned long dernierEnregistrement = 0; 
unsigned long intervalleEnregistrement = 5000; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nConnexion a " + String(ssid));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connecte. IP de l'application :");
  Serial.println(WiFi.localIP()); 
  server.begin();
}

void loop() {
  unsigned long tempsActuel = millis();
  
  if (tempsActuel - dernierEnregistrement >= intervalleEnregistrement) {
    dernierEnregistrement = tempsActuel;
    int valeur = analogRead(A0);
    int pct = map(valeur, 1024, 300, 0, 100);
    if(pct < 0) pct = 0;
    if(pct > 100) pct = 100;

    for(int i = 0; i < TAILLE_HISTORIQUE - 1; i++) {
      historique[i] = historique[i+1];
    }
    historique[TAILLE_HISTORIQUE - 1] = pct;
  }

  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            
            // 1. PREPARATION DES DONNEES
            String donneesGraphique = "[";
            for(int i=0; i<TAILLE_HISTORIQUE; i++) {
              donneesGraphique += String(historique[i]);
              if(i < TAILLE_HISTORIQUE - 1) donneesGraphique += ",";
            }
            donneesGraphique += "]";

            int pctActuel = historique[TAILLE_HISTORIQUE - 1];
            
            // --- LOGIQUE D'ANALYSE DÉTAILLÉE ---
            String diagnostic = "";
            String alerteCouleur = "";
            String conseilPro = "";

            if (pctActuel <= 20) {
                diagnostic = "❌ DANGER : SÉCHERESSE CRITIQUE";
                alerteCouleur = "#ff4444";
                conseilPro = "Action : Irrigation immédiate requise pour sauver la culture.";
            } 
            else if (pctActuel > 20 && pctActuel <= 70) {
                diagnostic = "✅ ÉTAT : OPTIMAL";
                alerteCouleur = "#00C851";
                conseilPro = "Action : Aucune. Les conditions de croissance sont parfaites.";
            }
            else if (pctActuel > 70 && pctActuel <= 85) {
                diagnostic = "⚠️ VIGILANCE : HUMIDITÉ ÉLEVÉE";
                alerteCouleur = "#ffbb33";
                conseilPro = "Action : Surveillez la météo avant d'arroser à nouveau.";
            }
            else { 
                diagnostic = "🚫 DANGER : SATURATION / INONDATION";
                alerteCouleur = "#CC0000";
                conseilPro = "Action : Arrêt total de l'irrigation. Risque d'asphyxie racinaire.";
            }

            // 2. ENVOI DE LA PAGE HTML
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html; charset=utf-8");
            client.println();
            
            client.print("<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
            client.print("<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>");
            client.print("<style>");
            client.print("body { font-family: sans-serif; background-color: #f4f4f4; padding: 15px; text-align: center; }");
            client.print(".card { background: white; padding: 20px; border-radius: 15px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); margin-bottom: 20px;}");
            client.print(".gros-chiffre { font-size: 45px; font-weight: bold; color: " + alerteCouleur + "; }");
            client.print("</style></head><body>");
            
            client.print("<h1>MBAY TECH 🌾</h1>");
            
            // Bloc 1 : Valeur actuelle
            client.print("<div class='card'>");
            client.print("<h2>Humidité Actuelle</h2>");
            client.print("<div class='gros-chiffre'>" + String(pctActuel) + "%</div>");
            client.print("</div>");

            // Bloc 2 : L'Analyse Intelligente
            client.print("<div class='card' style='border-top: 10px solid " + alerteCouleur + ";'>");
            client.print("<h2>Analyse du Sol</h2>");
            client.print("<div style='font-size: 20px; font-weight: bold; color: " + alerteCouleur + ";'>" + diagnostic + "</div>");
            client.print("<p style='font-size: 16px; color: #555; background: #f9f9f9; padding: 10px; border-radius: 5px;'>" + conseilPro + "</p>");
            client.print("<div style='text-align: left; font-size: 12px; margin-top: 10px; color: #888;'>");
            client.print("• 0-20%: Sec | 20-70%: Idéal | 85-100%: Excès");
            client.print("</div></div>");

            // Bloc 3 : Le Graphique
            client.print("<div class='card'>");
            client.print("<h2>Évolution (Temps Réel)</h2>");
            client.print("<canvas id='monGraphique'></canvas>");
            client.print("</div>");

            // SCRIPTS
            client.print("<script>");
            client.print("var ctx = document.getElementById('monGraphique').getContext('2d');");
            client.print("var myChart = new Chart(ctx, { type: 'line', data: {");
            client.print("labels: ['-45s','-40s','-35s','-30s','-25s','-20s','-15s','-10s','-5s','Maint.'],");
            client.print("datasets: [{ label: 'Humidité (%)', data: " + donneesGraphique + ",");
            client.print("borderColor: '#007bff', backgroundColor: 'rgba(0, 123, 255, 0.2)', fill: true, tension: 0.4 }]");
            client.print("}, options: { animation: false, scales: { y: { min: 0, max: 100 } } } });");
            client.print("setTimeout(function(){location.reload()}, 4000);");
            client.print("</script></body></html>");
            
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}