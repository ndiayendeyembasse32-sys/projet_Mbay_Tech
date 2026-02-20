# 🌾 MBAY TECH - Assistant d'Irrigation Intelligent

Projet IoT réalisé par Ndeye Mbasse Ndiaye - 6 Mars 2026

Mbay Tech est une solution technologique conçue pour optimiser l'irrigation agricole au Sénégal. Ce prototype utilise l'Internet des Objets (IoT) pour surveiller l'humidité du sol et fournir une aide à la décision en temps réel sur smartphone.

Fonctionnalités Clés
- **Lecture Temps Réel :** Mesure continue du taux d'humidité via une sonde capacitive.
- **Analyse Intelligente :** Algorithme qui interprète les données (Sécheresse, Optimal, Saturation) et donne des conseils agronomiques.
- **Historique Graphique :** Visualisation de l'évolution de l'humidité sur les dernières minutes grâce à Chart.js.
- **Dashboard Multi-Zones :** Simulation de profils spécifiques pour le Mil, l'Arachide et le Maraîchage.

 Architecture Technique


- **Microcontrôleur :** ESP8266 (NodeMCU) servant de serveur Web embarqué.
- **Capteur :** Sonde d'humidité du sol YL-69.
- **Réseau :** Connexion Wi-Fi locale (Mode Station via partage de connexion).
- **Interface :** Web App responsive développée en HTML/CSS/JavaScript (Chart.js).

 Logique de Décision
Le système utilise des seuils critiques pour alerter l'utilisateur :
- **0 - 20% :** 🔴 Danger Sécheresse (Irrigation immédiate).
- **20 - 70% :** 🟢 État Optimal (Conditions idéales).
- **70 - 85% :** 🟠 Vigilance (Humidité élevée).
- **85 - 100% :** 🛑 Danger Saturation (Risque d'asphyxie des racines).

 Installation et Test
1. Téléverser le code `MbayTech_Final.ino` sur la carte.
2. Connecter le montage à un **Power Bank** pour l'autonomie.
3. Se connecter au Wi-Fi `iPhone` (mot de passe : `ndeya226`).
4. Accéder à l'interface via l'adresse IP affichée sur le moniteur série.


Ce projet démontre l'application de l'Edge Computing pour une agriculture de précision durable.
