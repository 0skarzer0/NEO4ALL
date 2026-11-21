# HANDOFF - NEO4ALL

Estado a 2026-11-21. Para quien retome el trabajo (persona o IA).

## Qué es
Código fuente de NEO4ALL publicado por Chui, descomprimido y con la historia de versiones (un commit y una etiqueta por paquete: `alpha-one`, `alpha-two`, `beta-one`, `beta-2`, `beta-3`, `beta-4`, `rc1`, `rc3-fix`, `rc4`).

## Cómo se hizo
- Los paquetes se extrajeron en el repositorio [Chui-s-Dreamcast-Projects.](https://github.com/0skarzer0/Chui-s-Dreamcast-Projects.) (carpeta `NEO4ALL/src`), un commit por paquete, y se verificó cada commit contra el paquete original (`diff -r`).
- Este repositorio sale de ahí con `git filter-repo --subdirectory-filter NEO4ALL/src`; los árboles de cada commit son idénticos a los de ese repositorio (comprobado por hash).
- Diferencias con los paquetes: se quitó la carpeta contenedora cuando era única y se omitieron ficheros temporales de editor (`.swp`). Git no guarda carpetas vacías, así que no están las vacías de los paquetes.
- Las fechas de los commits son las reales de creación; la fecha original está en el mensaje.
- `convertneo` aparece en RC3; antes, `neo4all/` era el único directorio. Por eso el árbol de `neo4all/` se mantiene en todas las versiones.
- No existe RC2 en la web original.

## Pendiente / ideas
- No se ha compilado ninguna versión; el primer paso útil es comprobar que compila con KallistiOS y documentar la toolchain exacta.
- Si se trabaja aquí (cambios, ramas), mantener `main` con las etiquetas de versión como referencia del código original.
- No se crean releases de GitHub salvo que el propietario lo pida.
