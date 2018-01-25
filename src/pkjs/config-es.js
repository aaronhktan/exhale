module.exports = [
	{
		"type": "heading",
		"defaultValue": "Ajustes"
	},
	{
		"type": "text",
		"defaultValue": "Esta es la página de ajustes para la app Breathe. Estás usando la versión 2.53 de la app.",
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "Colores"
			},
			{
				"type": "color",
				"messageKey": "backgroundColor",
				"defaultValue": "000000",
				"label": "Color de fondo",
				"sunlight": true,
				"layout": "BLACK_WHITE"
			},
			{
				"type": "color",
				"messageKey": "circleColor",
				"defaultValue": "00AAFF",
				"label": "Color del círculo",
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
				"defaultValue": "En la app"
			},
			{
				"type": "toggle",
				"messageKey": "rememberDuration",
				"defaultValue": false,
				"label": "¿Recordar la duración anterior y usarla cuando se inicia la app?",
			},
			{
				"type": "toggle",
				"messageKey": "vibrationEnabled",
				"defaultValue": true,
				"label": "¿Activar la reacción háptica durante la respiración?",
			},
			{
				"type": "select",
				"messageKey": "vibrationType",
				"defaultValue": "0",
				"label": "Tipo de vibración",
				"description": "Repentina es una serie de vibraciones rápidas; Toques es un doble toque.",
				"options": [
					{
						"label": "Repentina - inhalación solamente",
						"value": "0"
					},
					{
						"label": "Repentina - inhalación y exhalación",
						"value": "1"
					},
					{ 
						"label": "Toques", 
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
				"label": "¿Dejar que el reloj escoga la velocidad de respiraciones automaticamente según el ritmo cardiaco?",
				"description": "NOTA: Esta opción requiere un reloj con un monitor de ritmo cardiaco."
			},
			{
				"type": "slider",
				"messageKey": "breathsPerMinute",
				"defaultValue": 7,
				"label": "Respiraciones por minuto",
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
			 	"defaultValue": "Recordatorios"
			},
			{
				"type": "select",
				"messageKey": "reminderHours",
				"defaultValue": "4",
				"label": "Recuérdame respirar a...",
				"description": "El app te recordará a este intervalo diariamente.",
				"options": [
					{ 
						"label": "Cada hora", 
						"value": "1" 
					},
					{ 
						"label": "Cada 2 horas", 
						"value": "2" 
					},
					{ 
						"label": "Cada 3 horas",
						"value": "3" 
					},
					{ 
						"label": "Cada 4 horas",
						"value": "4" 
					},
					{ 
						"label": "Cada 6 horas",
						"value": "6" 
					},
					{ 
						"label": "No me recuerda nunca",
						"value": "0" 
					}
				]
			},
			{
				"type": "slider",
				"messageKey": "reminderHoursStart",
				"defaultValue": 8,
				"label": "¿A qué hora debería empezar de recordarte la app?",
				"description": "De esta hora (es decir, 8 = las 8 de la mañana) hasta 12 horas trás, la app de recordará de respirar.",
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
				"label": "Mostrar la App Glance en el menú de las aplicaciónes",
			},
			{
				"type": "select",
				"messageKey": "appGlanceType",
				"defaultValue": "0",
				"label": "Tipo de App Glance",
				"options": [
					{
						"label": "Último duración de la sesión",
						"value": "0"
					},
					{
						"label": "Total hoy",
						"value": "1"
					},
					{
						"label": "Racha",
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
				"defaultValue": "Salud"
			},
			{
				"type": "select",
				"messageKey": "displayText",
				"defaultValue": "1",
				"label": "Mostrar...",
				"description": "Esto determina lo que está mostrado en el menú principal. NOTA: El ritmo cardiaco requiere un reloj con un monitor de ritmo cardiaco.",
				"options": [
					{
						"label": "Nada",
						"value": "0"
					},
					{
						"label": "Saludo",
						"value": "1"
					},
					{ 
						"label": "Pasos hoy", 
						"value": "2" 
					},
					{ 
						"label": "Ritmo cardiaco",
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
				"defaultValue": "Logros",
			},
			{
				"type": "toggle",
				"messageKey": "achievementsEnabled",
				"defaultValue": true,
			"label": "¿Activar los logros?"
			},
			{
				"type": "select",
				"messageKey": "bottomTextType",
				"defaultValue": "0",
				"label": "Texto mostrado en la parte inferior del menú principal",
				"options": [
					{
						"label": "Total hoy",
						"value": "0"
					},
					{
						"label": "Racha",
						"value": "1"
					}
				]
			},
			{
				"type": "input",
				"messageKey": "achievementsBackup",
				"defaultValue": "",
				"label": "Copia de seguridad y restauración de logros",
				"description": "El contenido del campo de texto te permite de hacer una copia de seguridad de tus logros, tus récordes, y cuántos minutos ya respiraste con esta app. ¡Guárdalo en un lugar seguro! Si quieres restaurar con un texto anteriormente guardado, pégalo en el campo de texto, pues puntea el botón 'Guardar ajustes'."
			},
			{
				"type": "button",
				"primary": false,
				"defaultValue": "Reiniciar logros",
				"id": "resetAchievementsButton",
			}
		]
	},
	{
		"type": "text",
		"defaultValue": "<em><center>Muchas gracias a los probadores beta y los contribuyentes: <br>Paula&nbsp;Bosca, Nikita&nbsp;Cheng, Ayush&nbsp;Gupta, Ellen&nbsp;Huang, Yvonne&nbsp;Tan, David&nbsp;Voicu, /u/dryingsocks, /u/JohnEdwa, y /u/PiwwowPants</center></em>",
	},
	{
		"type": "submit",
		"defaultValue": "Guardar mis ajustes"
	}
];