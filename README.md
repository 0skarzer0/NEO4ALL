# NEO4ALL

Emulador de NEOGEO/CD de código abierto para Dreamcast y Dingoo, de Chui y Fox68k.

NEO4ALL es un emulador alternativo de NEOGEO/CD para Dreamcast y la consola Dingoo, obra de **Chui** y **Fox68k**. Este repositorio contiene su código fuente, descomprimido y con la historia de versiones.

> Archivo de preservación, no el desarrollo original. Los commits **no son de Chui**: cada uno reconstruye un paquete de fuentes publicado en su web.

## Contenido

- `neo4all/` – el emulador (`Makefile.dc`, `Makefile.dingux`, `Makefile.linux`, `Makefile.windows`, `config.mk`, `data/`, `src/`).
- `convertneo/` – herramienta de línea de comandos para convertir ROMs (desde RC3; `Makefile`, `Makefile.linux`, `Makefile.win32`).

## Características

- Emulación casi a velocidad completa (frameskip 0 con efectos de sonido).
- Núcleo FAME (Motorola 68000) y FAZE (Zilog Z80), de Fox68k.
- Núcleo gráfico propio para PowerVR2 (caché rápida de tiles).
- Autoframeskip, menú completo (región, frameskip, filtro gráfico, reinicio), sonido sin retardo y los dos jugadores.
- Tarjeta SD para cargar imágenes ISO/BIN/MDF.

## AES4ALL

AES4ALL (NEOGEO AES/MVS en lugar de NEOGEO/CD) **no tiene código propio: es este mismo código compilado en modo AES.** El código del modo AES está en `neo4all/src/aes/` desde **RC1** y `convertneo/src/neoaes.c` (conversor de ROMs a `.aes`); se activa con la variable `AES` de `neo4all/config.mk` (línea `#AES=1`), que da el ejecutable `aes4all`. No se ha compilado para comprobarlo ni se sabe de qué versión salió cada binario de AES4ALL publicado.

## Historia de versiones

Un commit por paquete publicado, en orden cronológico (por la fecha de los ficheros de cada paquete), con una etiqueta por versión. Los commits llevan la fecha en que se crearon, no la de 2004-2011. El mensaje de cada commit incluye el SHA-256 del paquete original. Para comparar dos versiones: `git diff rc3-fix rc4`.

| Etiqueta | Versión | Paquete original | Fecha del paquete |
|---|---|---|---|
| `alpha-one` | alpha one | `neo4all-src-alpha-one.rar` | 2005-02-07 |
| `alpha-two` | alpha two | `neo4all-src-alpha-two.rar` | 2005-02-14 |
| `beta-one` | beta one | `neo4all-src-beta-one.rar` | 2005-02-27 |
| `beta-2` | beta 2 | `neo4all-src-beta2.tar.bz2` | 2005-04-11 |
| `beta-3` | beta 3 | `neo4all-src-beta3.tar.bz2` | 2005-10-10 |
| `beta-4` | beta 4 | `neo4all-src-beta4.tar.bz2` | 2005-11-13 |
| `rc1` | RC1 | `neo4all-src-rc1.tar.bz2` | 2006-03-11 |
| `rc3-fix` | RC3 (fix) | `neo4all-src-rc3-fix.tar.bz2` | 2007-04-12 |
| `rc4` | RC4 | `neo4all-src-rc4.tar.bz2` | 2011-10-02 |

## Compilar

Para Dreamcast se compila con [KallistiOS](https://github.com/KallistiOS/KallistiOS) usando `neo4all/Makefile.dc`; hay también Makefiles para Dingoo (`Makefile.dingux`), Linux y Windows. **Este repositorio no ha compilado ninguna versión**: es un archivo del código publicado.

## Origen

- Web original: *Chui's DC projects* (`chui.dcemu.co.uk`), recuperada de Internet Archive. Copia de la web con todos los proyectos: [Chui-s-Dreamcast-Projects.](https://github.com/0skarzer0/Chui-s-Dreamcast-Projects.) ([ver la página de NEO4ALL](https://htmlpreview.github.io/?https://github.com/0skarzer0/Chui-s-Dreamcast-Projects./blob/main/NEO4ALL/Chuis_DC_projects_NEO4ALL.html)).
- Los paquetes originales (`*.tar.bz2`, `*.rar`) siguen en ese repositorio, en `NEO4ALL/archivos/`. Esta historia se extrajo de allí con `git filter-repo` (`NEO4ALL/src`); por eso los mensajes de commit mencionan rutas de ese repositorio.

## Créditos

- **Chui** – autor de NEO4ALL y de la web original (*Chui's DC projects*, `chui.dcemu.co.uk`).
- **Fox68k** – núcleos FAME y FAZE, y coautor según la web.
- **GnGeo** (Mathieu Peponas, «Copyright (C) 2001 Peponas Mathieu», GPL) – base de `convertneo` y de parte de `neo4all/src`.
- **Stéphane Dallongeville** (copyright 2004-2005) y **Juergen Buchmueller** – ficheros de terceros en `neo4all/src`.
- **KallistiOS** – SDK de Dreamcast con el que se compila.
- **Internet Archive** (Wayback Machine) – de donde se recuperó la web.

## Licencia

Este repositorio no añade ninguna licencia. El código mezcla componentes con licencias distintas, cuyos avisos se conservan en cada fichero: GPL (GnGeo en `convertneo/src`, y 2 ficheros de `neo4all/src`) y ficheros de «uso libre para fines no comerciales» (8 ficheros de `neo4all/src`). Para uso comercial hay que consultar a los titulares de cada parte.
