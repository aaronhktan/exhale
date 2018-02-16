module.exports = [
  {
    "type": "heading",
    "defaultValue": "Paramètres"
  },
  {
    "type": "text",
    "defaultValue": "Ceci est la page de configuration pour l'app Breathe. Vous utilisez version 2.53 de l'app.",
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Couleurs"
      },
      {
        "type": "color",
        "messageKey": "backgroundColor",
        "defaultValue": "000000",
        "label": "Couleur de fond",
        "sunlight": true,
        "layout": "BLACK_WHITE"
      },
      {
        "type": "color",
        "messageKey": "circleColor",
        "defaultValue": "00AAFF",
        "label": "Couleur du cercle",
        "sunlight": true,
        "capabilities": ["COLOR"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Dans l'app"
      },
      {
        "type": "toggle",
        "messageKey": "rememberDuration",
        "defaultValue": false,
        "label": "Se rappeler de la durée précédente?",
      },
      {
        "type": "toggle",
        "messageKey": "vibrationEnabled",
        "defaultValue": true,
        "label": "Activer les vibrations pendant les respirations?",
      },
      {
        "type": "select",
        "messageKey": "vibrationType",
        "defaultValue": "0",
        "label": "Type de vibration",
        "description": "Les pulsations rapides sont une série de vibrations rapides; Tapes tape deux fois.",
        "options": [
          {
            "label": "Pulsations rapides - inhalation seulement",
            "value": "0"
          },
          {
            "label": "Pulsations rapides - inhalation et exhalation",
            "value": "1"
          },
          { 
            "label": "Tapes", 
            "value": "2"
          }
        ]
      },
      {
        "type": "toggle",
        "capabilities": [
          "NOT_PLATFORM_APLITE", 
          "NOT_PLATFORM_BASALT",
          "NOT_PLATFORM_CHALK"
          ],
        "messageKey": "heartRateVariation",
        "defaultValue": false,
        "label": "Choisir la vitesse de respiration selon le rhythme cardiaque?",
        "description": "Note: Ceci requiert une montre avec un moniteur cardiaque."
      },
      {
        "type": "slider",
        "messageKey": "breathsPerMinute",
        "defaultValue": 7,
        "label": "Respirations par minute",
        "min": 2,
        "max": 10,
        "step": 1
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Rappels"
      },
      {
        "type": "select",
        "messageKey": "reminderHours",
        "defaultValue": "4",
        "label": "Me rappeler de respirer",
        "description": "L'app vous rappellera de respirer à cet intervalle tous les jours.",
        "options": [
          { 
            "label": "Toutes les heures", 
            "value": "1" 
          },
          { 
            "label": "Toutes les 2 heures", 
            "value": "2" 
          },
          { 
            "label": "Toutes les 3 heures",
            "value": "3" 
          },
          { 
            "label": "Toutes les 4 heures",
            "value": "4" 
          },
          { 
            "label": "Toutes les 6 heures",
            "value": "6" 
          },
          { 
            "label": "Ne me rappelle pas",
            "value": "0" 
          }
        ]
      },
      {
        "type": "slider",
        "messageKey": "reminderHoursStart",
        "defaultValue": 8,
        "label": "Quand devraient commencer les rappels?",
        "description": "Dès cette heure du matin (c.à.d 8 = 8h00 du matin) et pour un période 12 heures de suite, l'app vous rappellera de respirer.",
        "min": 6,
        "max": 10,
        "step": 1
      }
    ]
  },
  {
    "type": "section",
    "capabilities": ["NOT_PLATFORM_APLITE"],
    "items": [
      {
        "type": "heading",
        "defaultValue": "App Glance",
      },
      {
        "type": "toggle",
        "messageKey": "appGlanceEnabled",
        "defaultValue": true,
        "label": "Afficher App Glance dans le menu des apps",
      },
      {
        "type": "select",
        "messageKey": "appGlanceType",
        "defaultValue": "0",
        "label": "Type d'App Glance",
        "options": [
          {
            "label": "Durée de la dernière session",
            "value": "0"
          },
          {
            "label": "Total aujourd'hui",
            "value": "1"
          },
          {
            "label": "Série",
            "capabilities": ["NOT_PLATFORM_APLITE"],
            "value": "2"
          }
        ]
      },
    ]
  },
  {
    "type": "section",
    "capabilities": ["HEALTH"],
    "items": [
      {
        "type": "heading",
        "defaultValue": "Santé"
      },
      {
        "type": "select",
        "messageKey": "displayText",
        "defaultValue": "1",
        "label": "Montrer...",
        "description": "Ceci détermine ce qui est montré en haut du menu principal. Montrer le rhythme cardiaque requiert une montre avec un moniteur cardiaque.",
        "options": [
          {
            "label": "Rien",
            "value": "0"
          },
          {
            "label": "Salut",
            "value": "1"
          },
          { 
            "label": "Nombre de pas pris aujourd'hui", 
            "value": "2" 
          },
          { 
            "label": "Rhythme cardiaque",
            "capabilities": ["PLATFORM_DIORITE", "PLATFORM_EMERY"],
            "value": "3" 
          }
        ]
      },
    ]
  },
  {
    "type": "section",
    "capabilities": ["NOT_PLATFORM_APLITE"],
    "items": [
      {
        "type": "heading",
        "defaultValue": "Succès",
      },
      {
        "type": "toggle",
        "messageKey": "achievementsEnabled",
        "defaultValue": true,
      "label": "Activer les succès?"
      },
      {
        "type": "select",
        "messageKey": "bottomTextType",
        "defaultValue": "0",
        "label": "Texte en bas du menu principal",
        "options": [
          {
            "label": "Total aujourd'hui",
            "value": "0"
          },
          {
            "label": "Durée de jours consécutives respirés",
            "value": "1"
          }
        ]
      },
      {
        "type": "input",
        "messageKey": "achievementsBackup",
        "defaultValue": "",
        "label": "Sauvegarder et récupération des succès",
        "description": "Le texte dans l'encadré vous permet de sauvegarder les succès que vous avez gagnés, vos records de série, ainsi que le montant total de minutes que vous avez respirés avec cette app. Garde-le quelque part sécuritaire! Si vous voulez récupérer vos succès antérieures, collez le texte déjà gardé puis appuyez sur le bouton 'Sauvegarder mes paramètres'."
      },
      {
        "type": "button",
        "primary": false,
        "defaultValue": "Réinitialiser les succès",
        "id": "resetAchievementsButton",
      }
    ]
  },
  {
    "type": "text",
    "defaultValue": "<em><center>Un grand merci aux bêta-testeurs et contributeurs: <br>Paula&nbsp;Bosca, Nikita&nbsp;Cheng, Ayush&nbsp;Gupta, Ellen&nbsp;Huang, Yvonne&nbsp;Tan, David&nbsp;Voicu, /u/dryingsocks, /u/JohnEdwa, et /u/PiwwowPants</center></em>",
  },
  {
    "type": "submit",
    "defaultValue": "Sauvegarder mes paramètres"
  }
];