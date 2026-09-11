# Notas de Versión

## v0.0.3

**Fecha:** 2026-09-11
**Tag:** `v0.0.3`

### Gameplay changes:

- Último disparo lanza solo la bola sin cadena (SpawnBall)
- Se deshabilita la colisión del cañón/jugador (solo lógica, sin hitbox)
- Se cambia sprite del cañón por la mano de un gigante
- Se ajusta la rotación cuando de la mano cuando está apuntando
- Se mejora la UI del menú princial usando Raygui
- Se cambia el comando para habilitar el modo debug: ahora hay que tocar "D" cinco veces

### Fixes:

- Fix: totalShotsFired se acumulaba cada frame al ganar un nivel (se corrigió en HandleLevelEnd)
- Fix: estados de botones del menú no se reseteaban entre frames
- Fix: concatenación de strings en GameUI (std::string wrapper y \n faltantes)

## v0.0.2 — Refinamiento Visual

**Fecha:** 2026-09-04  
**Tag:** `v0.0.2`

### Agregado

- Sprites para el fondo del escenario
- Sprites para cadena, bola y cañón

---

## v0.0.1 — Mecánicas Completas

**Fecha:** 2026-09-03  
**Tag:** `v0.0.1`

### Agregado

- Cañón con rotación y movimiento vertical
- Cadena con eslabones y bola demoledora
- Construcción de edificios a partir de bloques apilados programáticamente
- Detección de caída y condición de victoria/derrota
- Sistema de niveles con patrones de construcción
- Texturas en bloques del edificio programática usando un tilemap por nivel
- Paredes prismáticas como obstáculos
- Sistema de partículas al colisionar
- Interfaz con HUD (nivel, altura, meta, disparos)
