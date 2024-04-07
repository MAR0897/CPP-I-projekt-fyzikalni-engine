# Fyzikální engine – checkpoint projektu

## Report:

Co se týče funkcionalit, zatím se mi podařilo naimplementovat jednoduché vkládání, posouvání, otáčení a zvětšování/zmenšování těles na „pracovní ploše“ enginu. Se změnami těles se taktéž mění jejich fyzikální vlastnosti (třeba zvětšením se zvětší hmotnost). Tělesa jsou zatím koule, obdélník (který zatím jde vložit pouze jako čtverec) a rovnostranný trojúhelník. Plánuju přidat i obecný mnohoúhelník. 

Dále funguje gravitace, realistická kolize dvou objektů a omezené míře i otáčení (při kolizi normálního a statického (=nepohnutelného) tělesa se tam nějaká chyba ve výpočtu). Tření a odpor vzduchu jsou zatím jako ve většině fyzikálních úloh (neexistují) (ale to tření plánuju přidat). O radiálním gravitačním poli těles ani nelze mluvit 

Funguje i automatické odstraňování těles, když se dostanou „pod zem“. Tělesa lze také přeměnit na statická. 

Ve finální verzi bych rád nějak zoptimalizoval výpočty, pokud to půjde, a vytvořil možná nějaký UI na přidávání objektů a statistiky (nevím, jak moc je to realistické) a nějaké další fyzikální funkcionality jako to tření nebo třeba nějaké spojování těles. Nevím, kolik moc se toho po mně chce, už teď ten kód je relativně dost dlouhý :D (ale třeba je to jenom špatným designem?).

## Spuštení:

Je třeba nainstalovat OpenGL, např. takto:

```
sudo apt install libglfw3-dev libglew-dev
```

dále:

```
cmake .
make
./main
```

## Ovládání:

- Tělesa se přidávají stisknutím R, T, C (rectangle, triangle, circle). Jejich střed bude přesně v místě, kde ukazuje kurzor. 
- Kliknutím na ně se dají do módu „selected“ a může se s nimi hýbat (šipky), otáčet (A, D), zvětšovat/zmenšovat (scroll). 
- Těleso taky kdykoliv může přejít do „static“ módu stisknutím S (kurzor musí být uvnitř tělesa).
- Deletem se těleso odstraní (pokud není statické).
- G se togglne gravitace (defaultně není zapnutá)
- ESC se okno zavře