module.exports = [
	{
		"type": "heading",
		"defaultValue": "Paramètres"
	},
	{
		"type": "text",
		"defaultValue": "Ceci est la page de configuration pour l'app Breathe. Vous utilisez version 0.2.11 de l'app.",
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
				"defaultValue": "00AA55",
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
				"description": "Secousses est comme celui du Apple Watch; Tapes tape deux fois.",
				"options": [
					{
						"label": "Secousses",
						"value": "0"
					},
					{ 
						"label": "Tapes", 
						"value": "1"
					}
				]
			},
			{
				"type": "slider",
				"messageKey": "breathsPerMinute",
				"defaultValue": 7,
				"label": "Respirations par minute",
				"min": 4,
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
						"label": "Salut",
						"value": "0"
					},
					{ 
						"label": "Nombre de pas pris aujourd'hui", 
						"value": "1" 
					},
					{ 
						"label": "Rhythme cardiaque",
						"value": "2" 
					}
				]
			},
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
		"type": "text",
		"defaultValue": "<em><center>Un grand merci aux bêta-testeurs: <br>Paula&nbsp;Bosca, Nikita&nbsp;Cheng, Ayush&nbsp;Gupta, Ellen&nbsp;Huang, Yvonne&nbsp;Tan, David&nbsp;Voicu, et /u/PiwwowPants</center></em>",
	},
	{
		"type": "submit",
		"defaultValue": "Sauvegarder mes paramètres"
	}
];