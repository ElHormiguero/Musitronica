# Musitronica

Aprovechando el sonido de diferentes elementos electrónicos y mediante la secuenciación a través del ordenador, creamos ritmos y melodías que reproducen la canción de Memories, Don't leave me alone y Like do it de David guetta.

Los relés conectados a un contador y a 8 bombillas reproducen un "clic" acompañado de un espectáculo de luz.
Los solenoides golpean dos vasos simulando la caja y el bombo.
Los motores paso a paso generan vibraciones al moverse que dan lugar a armónicos que dependen de la frecuencia de los pasos.

Todo se gestiona desde un microcontrolador con protocolo de comunicación midi que recibe las notas por diferentes canales desde un programa musical y los traduce en alto y bajo de las salidas digitales conectadas a los relés y a 4 interrupciones de diferentes periodos que controlan los pulsos enviados a los driver de los motores.

