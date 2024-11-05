Nombre d'Arduino Requis

Pour gérer l'ensemble des éléments sans surcharge sur un seul Arduino et en garantissant une flexibilité de développement, je recommande l'utilisation de 4 Arduino Mega :

    Arduino Mega 1 : Gestion des Servos et Électroaimant
        Servos : horloge, cadre, boîte.
        Électroaimant : Pour l'ouverture de la porte de sortie.

    Arduino Mega 2 : Gestion des Livres et de la Figurine
        Livres (1-6) : Entrées représentant les livres que les joueurs doivent appuyer.
        Moteur Pas à Pas pour la Figurine : Pour déplacer la figurine vers une position définie.

    Arduino Mega 3 : Gestion du Clavier et MQTT
        Clavier : Utilisé pour entrer le code d'accès.
        MQTT : Pour la communication avec un serveur MQTT, gérer la coordination entre les éléments et l'activation de la vidéo.

    Arduino Mega 4 : Gestion du Feedback Utilisateur
        Écran LCD : Pour afficher les instructions et les statuts aux joueurs.
        Capteur de Fin de Course : Pour détecter la position de la figurine et déclencher la séquence.

Connexions des Éléments
Arduino Mega 1 (Gestion des Servos et Électroaimant)

    Horloge Servo : Broche PWM 8
    Cadre Servo : Broche PWM 9
    Boîte Servo : Broche PWM 10
    Électroaimant : Broche numérique 11 (via un transistor pour contrôler la haute puissance)

Arduino Mega 2 (Gestion des Livres et de la Figurine)

    Livres (comme entrées) :
        Livre 1 : Broche 2
        Livre 2 : Broche 3
        Livre 3 : Broche 4
        Livre 4 : Broche 5
        Livre 5 : Broche 6
        Livre 6 : Broche 7
    Moteur pour Figurine :
        STEP_PIN : Broche 22
        DIR_PIN : Broche 23
        Capteur de Fin de Course (détection de position) : Broche 8 (pour savoir quand la figurine touche la position cible)

Arduino Mega 3 (Gestion du Clavier et MQTT)

    Clavier :
        Lignes (Rows) : A0, A1, A2, A3
        Colonnes (Cols) : A4, A5, A6, A7
    Module Ethernet (pour MQTT) :
        SPI (MISO, MOSI, SCK, CS) : Broches 50, 51, 52, 53

Arduino Mega 4 (Gestion du Feedback Utilisateur)

    Écran LCD :
        RS : Broche 50
        EN : Broche 51
        D4, D5, D6, D7 : Broches 49, 48, 47, 46
    Buzzer ou Module Audio : Pour fournir un feedback sonore supplémentaire si nécessaire.