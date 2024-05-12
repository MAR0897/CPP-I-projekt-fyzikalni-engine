# Projekt C++ I, LS 2024

### Téma – Fyzikální engine

## Popis projektu

Cílem tohoto projektu bylo vytvořit program, který simuluje reálné fyzikální interakce mezi objekty – posuvný pohyb, otáčivý pohyb, kolize objektů, tření a další fyzikální věcičky. Samotný program je "pracovní plocha", na kterou uživatel může pokládat různé geometrické objekty a ty by pak měly interagovat s prostředím a ostatními objekty dle fyzikálních zákonů. Engine jede ve vektorovém prostoru o dimenzi 2 a všechny fyzikální veličiny jsou proto "naškálované" (třeba hmotnost objektu = hustota * obsah).

## Funkcionality

### Manipulace s objekty

- Přidávání objektů na pracovní plochu. Možné objekty jsou obdélník (Rectangle, zelený), trojúhelník (Triangle, žlutý), kruh (Circle, modrý), které se dají přidat stisknutím postupně R, T a C tam, kam ukazuje myš.
- Vybírání objektů kliknutím. Na objekt je možné kliknou myší (LMB) a tak ho "vybrat". Vybrané objekty se zobrazují červenou barvou. Dalším kliknutím na objekt ho deselectnem.
- Vybírání objektů swipem. Přetáhnutím kurzoru po pracovní ploše při stisku pravého tlačítka myši (RMB) se vyberou všechny objekty jejichž AABB (axis-aligned bounding box) zasahuje do přetáhnutého obdélníku.
- Deselect všech objektů stisknutím Q.
- Změna fyzikální vlastností objektů. Jestliže je objekt ve stavu "vybrán" jde jej zmenšovat/zvětšovat (mouse scroll), posouvat (šipky, aplikuje se síla), otáčet (A proti směru / D po směru). Zvětšování a zmenšováním se mění fyzikální vlastnosti (hmotnost, moment setrvačnosti).
- Static objekty – nehnutelné objekty – například zem. Jsou černé s bílým ohraničením. Dají se aktivovat/deaktivovat stisknutím S. Static objekty nemůžou být vybrané.
- Smazání objektů z plochy – Stisknutím DELETE se smažou všechny vybrané objekty.

### Fyzika

- Gravitace – stisknutím G se aktivuje/deaktivuje gravitace. Defaultně je vyplá.
- Kolize objektů – když se dva objekty srazí, upraví se jejich rychlosti, úhlové rychlosti a pozice. To navíc za působení tření. Funguje ale jen posuvné tření (to normální), valivý odpor a podobné vymoženosti implementovány nejsou.
- Hmotnost a moment setrvačnosti – každému objektu jsou tyto dvě veličiny vypočítávány na základě jeho tvaru a hustoty, která se dá změnit manuálně (popsáno níže).
- Statické a dynamické tření – tření v klidu a v pohybu
- Koeficient restituce – jak moc efektivně probíhá odraz (čím blíž k 1, tím víc skákavější)

### Ukládání do souboru a načítání ze souboru

Celou simulaci je možné se vším všudy (kromě gravitace) uložit do souboru a pak ji z něj načíst zpět. Zapisování do souboru probíhá stisknutím CTRL + číselno-háčkových-čárkových kláves (prostě ty pod funkcemi, ne ta čísla úplně napravo). Takhle se zapíše simulace do přednastaveného souboru test1 až test10.json. Načítání simulace se provádí stisknutím SHIFT + klávesa a zase se načítá z přednastavených souborů. Zapsání do a načtení z vlastního souboru lze stisknutím GRAVE_ACCENT (klávesa pod ESC). Je nutné zadat umístění souboru do terminálu.
Ukládání probíhá do JSON souboru. Data jsou uložená jako pole objektů. Je nutné proto mít i speciální knihovnu na parsování JSON souborů.
Manuálními změnami se tím pádem dá dostat třeba jiné tření nebo koeficient restituce, než jsou defaultně zadány.

### Další funkcionality

- Stisknutím ESC se engine zavře.
- Jestliže objekt (resp. jeho AABB) spadne pod monitor (ano, skutečně až pod monitor, ne pod "hranici" okna programu), objekt se smaže.
- V levém horním rohu se ukazuje počet simulovaných objektů
- Vybráním objektu se ukážou jeho hodnoty fyzikálních veličin. Při vybrání více objektů se ukazují jen statistiky objektu, který byl přidán jako první (dojde se k němu jako k prvnímu při iterování vektorem objektů)
- Objekty lze také přidávat mimo obrazovku namířením myši úplně mimo program a stisknutím R/T/C. (to nebylo chtěné)

## Spuštění:

Je třeba nainstalovat OpenGL, tedy GLFW, GLEW a GLUT a pak taky knihovnu na parsování JSON souborů, např. takto:

```
sudo apt-get install libglew-dev libglfw3-dev freeglut3-dev libjsoncpp-dev
```

dále stačí zkompilovat:

```
cmake .
make
```

a spustit:

```
./main
```

## Ještě jednou ovládání
- R/T/C – přidání obdélníku/trojúhelníku/kruhu
- šipky – pohyb vybraným/i objektem/ty
- A/D – rotace vybraným/i objektem/ty (směr: A = +, D = -)
- S – toggle statického tělesa
- Q – deselect všech těles
- G – toggle gravitace
- ESC – zavření enginu
- DELETE – smazání všech vybraných objektů
- CTRL + number keys – uložení simulace do přednastaveného souboru
- SHIFT + number keys – načtení simulace z přednastaveného souboru
- CTRL + GRAVE_ACCENT – uložení simulace do custom souboru
- SHIFT + GRAVE_ACCENT – načtení simulace z custom souboru
- LMB – vybrání objektu
- RMB – swipe select kliknutím a posunutím 
- SCROLL – změna velikosti vybraných objektů

## Testy
Zde jsou rozepsané simulace, které demonstrují funkčnost programu. Simulace jsou uložené v json/testX.json, kde X je číslo simulace. Stačí načíst stisknutím SHIFT + X (číslo), jak bylo popsáno v sekci ukládání.

1. Tělesa rozdílné hmotnosti dopadnou ve vakuu stejně
2. Triangle sort
3. Pyramid scheme a ilumináti
4. Wheeeeee (zkus větší úhlovou rychlost)
5. Nakloněná rovina
6. Rozdílné momenty setrvačnosti (něco jako hmotnost, ale u otáčivého pohybu) pro stejně hmotná tělesa (při rotaci, při které se aplikuje na všechna tělesa stejná síla, se jiné tvary budou otáčet jinak rychle)
7. Too many objects test (už je to pomalejší)
8. volný prostor
9. volný prostor
10. volný prostor