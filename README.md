#Weerstation

##Inleiding
Dit weerstation haalt alle weersinformatie voor jouw regio online op via een API van www.weerlive.nl. Jouw regio wordt bepaald aan de hand van de geografische coördinaten welke verkregen worden door de ingebouwde GPS ontvanger. Deze weersinformatie wordt elke 5 minuten vernieuwd. Mocht er een weerswaarschuwing gelden dan zal dit aangegeven worden met een rood knipperend uitroepteken. Druk op het uitroepteken om deze waarschuwing weer te geven. In dat zelfde scherm worden ook gedetecteerde bliksemontladingen weergegeven.
Voor radioamateurs is er ook een sectie ingericht waar relevante ruimteweersinformatie wordt weergegeven. Ook worden bandomstandigheden voor DX op korte golf en VHF getoond. Deze informatie wordt verkregen van hamqsl.com. De MUF informatie wordt verkregen van meteo.be. Deze gegevens worden respectievelijk elke 30 seconden en elke 20 seconden vernieuwd.
Tot slot is het weerstation uitgevoerd met een ingebouwde temperatuurmeter, luchtvochtigheidsmeter en een VOC meter. VOC geeft een indicatie van de luchtkwaliteit. VOC waardes onder 200 zijn prima, bij waardes boven 200 wordt ventilatie aanbevolen. Hoe hoger de waarde, hoe hoger de urgentie. Deze gegevens worden elke 5 seconden vernieuwd.
In een apart menu worden de GPS statistieken weergegeven. 

 
##Technische werking
Het hart van het weerstation is de ESP32 processor. Deze krachtige processor heeft ook een ingebouwd WiFi station. De software is geschreven op het welbekende Arduino platform. Software laden kan via de seriële poort J1. Hiervoor is wel een UART converter nodig. Om de ESP32 in de download stand te zetten dien je de spanning te verwijderen, de FLASH knop ingedrukt te houden en daarna de spanning terug te plaatsen. Hierna kun je de flash knop loslaten. Nu kun je de software laden.
Communicatie met de indoor temperatuur en luchtvochtigheid sensor loopt via een OneWire protocol. Dat wil zeggen dat alle data over één lijn loopt. Communicatie met de VOC en bliksemdetector loopt via de I2C databus. Tot slot is de GPS module gekoppeld middels een softwarematige seriële bus. 
Het Nextion display, dit is een HMI display wat dus zelfstandig werkt en dus ook eigen software heeft is aangesloten via de 2e seriële bus van de ESP32. Updates voor het Nextion scherm lopen via een SD kaart. Deze kan in de SD kaartlezer van het display worden geplaatst. Na het plaatsen dien je even kort de spanning los te halen. Daarna zal de update automatisch starten, de status wordt ook weergegeven op het display. Na de update kun je de SD kaart verwijderen en weer even kort de spanning los koppelen. Het display start daarna automatisch op met de nieuwe software.
De bliksemdetector analyseert ontvangen signalen op 500kHz met een bandbreedte van 33kHz. Aangezien dit vrij kritisch is, moet de capaciteit van de LC kring ingesteld worden voor een goede werking. Deze kan namelijk exemplarisch afwijken. In dit bouwproject wordt deze waarde automatisch gemeten en bepaald tijdens het opstarten.

##Handleiding
Dit weerstation is geheel afhankelijk van een werkende internet verbinding via Wi-Fi. Na het opstarten zal er eerst geprobeerd een verbinding te maken met het laatst verbonden Wi-Fi accesspoint. Mislukt dit, dan zal er een scherm verschijnen met de vraag om dit opnieuw te proberen. Druk je op het icoontje rechtsboven in het scherm, dan kun je eventueel de Wi-Fi opnieuw configureren.

#Wi-Fi opnieuw configureren
Je krijgt een QR code te zien. Ontgrendel je telefoon of tablet en scan de QR code. Je telefoon of tablet zal na een aantal seconden automatisch naar een configuratiepagina gaan waar je de Wi-Fi verbinding kunt configureren. Lukt dit niet, maak dan handmatig verbinding met het weerstation. De Wi-Fi-naam wordt onder de QR code getoond.

#API weerlive configureren
Zoals in de inleiding wordt geschreven maakt dit weerstation gebruik van de API van weerlive.nl. Het is dus belangrijk om een unieke code aan te vragen. Je kunt deze code gratis aanvragen door een account aan te maken op https://weerlive.nl/api/toegang/. Let wel op, het aantal aanvragen per dag is voor de gratis accounts beperkt tot 300. Maak dus voor elk weerstation een unieke code aan. Je kunt de code invullen tijdens het configureren van Wi-Fi. 

##Hoofdscherm
In het hoofdscherm vind je de huidige weersinformatie, de weersverwachting en binnentemperatuur/luchtvochtigheid en VOC waarde. Daarnaast wordt boven in het scherm de plaats  getoond welke horen bij de getoonde weersinformatie. Mocht er voldoende GPS ontvangst zijn, of is NTP ingeschakeld, dan zal daar ook de actuele datum en tijd worden weergegeven én, indien Hamradio mode is geactiveerd, de 10 digit (Maidenhead) QTH locator.

##Hamradio scherm
In het Hamradio scherm worden de nodige data van de zonneactiviteit weergegeven, daarnaast worden de verwachte bandcondities voor de korte golf, de actuele Europese bandcondities voor VHF en de actuele MUF voor verschillende afstanden op de korte golf getoond. Dit laatste wordt als een ticker weergegeven onderin beeld.

##GPS scherm
Hier worden de actuele lengte- en breedtegraad, het aantal ontvangen satellieten, de actuele GPS UTC tijd, de berekende hoogte (niet nauwkeurig) en een foutindicatie weergegeven.

##Waarschuwingenscherm
In het waarschuwingenscherm worden actuele weerswaarschuwingen weergegeven met een ticker. Daarnaast wordt ook een lijst met laatst gedetecteerde bliksemontladingen getoond, hierbij wordt datum, tijd en relatieve afstand weergegeven.

##Configuratiescherm
De volgende gegevens kun je wijzigen in het configuratiescherm:

De eerste pagina bevat instellingen voor de bliksemdetector.

###Alleen bij weeralarm
Indien ingeschakeld worden er alleen blikseminslagen gedetecteerd indien er weer weerswaarschuwing van kracht is. Dit kan eventuele valse meldingen aanzienlijk reduceren.

###Bliksem gevoeligheid
Hiermee kun je de gevoeligheid van de bliksemdetector instellen. Een lagere waarde is gevoeliger, maar ook gevoeliger voor valse meldingen.

###Spike onderdrukking
Hiermee wordt de gevoeiligheid voor storingen ingesteld. Een lagere waarde is gevoeliger, maar ook gevoeliger voor valse meldingen.

###Binnen gebruik
Hiermee wordt de gevoeligheid van de detector geoptimaliseerd. Bij buiten gebruik zijn de omgevingsstoringen anders dan bij binnen gebruik. 

 
##Configuratie pagina 2:

###Beeper
Indien deze functie is ingeschakeld wordt er een pieptoon gegeven indien er een weerswaarschuwing geldt of indien er een bliksemontlading is gedetecteerd.

###Hamradio modus
Indien deze functie is ingeschakeld wordt het hamradio scherm toegankelijk en zal de QTH locatie bij voldoende GPS signaal worden getoond.

###Helderheid
Instelling voor helderheid van het scherm. Instelbaar tussen 20 en 100%.

###Km/u weergave
Geef de actuele windsnelheid in km/u. Indien uitgeschakeld is de weergave in m/s.

##Configuratie pagina 3:

###Auto locatie update
Indien ingeschakeld wordt de gedetecteerde GPS locatie gebruikt voor de weersinformatie. Indien het weerstation op een plek wordt geplaatst waar GPS ontvangst onmogelijk of zeer matig is, wordt aangeraden deze functie na één keer goede ontvangst uit te schakelen. Hierdoor wordt de informatie niet meer bijgewerkt. Dit is belangrijk omdat mogelijk foute data als gevolg van slechte ontvangst kan worden gebruikt. Indien dit is uitgeschakeld, zal de QTH locator ook niet meer worden weergegeven.

###Gebruik NTP
Vooral handig indien er slechte of geen GPS ontvangst mogelijk is. De datum en tijd wordt, indien geactiveerd, via het internet gesynchroniseerd.

###Indoor offset
Hiermee kun je de waarde van de temperatuursensor voor binnentemperatuur wijzigen. Dit is vooral van belang indien de temperatuursensor op de print wordt geplaatst. Eigen opgewekte warmte van het weerstation kunnen namelijk de gemeten temperatuur beïnvloeden. Het is aan te raden deze waarde te wijzigen wanneer het weerstation minimaal een uur is ingeschakeld.

###Auto dimmer
Indien ingeschakeld zal het display na 5 minuten automatisch gedimd worden. Dit kan ongedaan worden door het scherm aan te raken.
Software

De software voor dit project is open source. Je kunt alle software en ook een 3d printbare behuizing vinden op mijn github pagina, https://github.com/PE5PVB/weerstation
Dit ontwerp is uitsluitend bedoeld voor zelfbouwtoepassing door hobbyisten. Commercieel gebruik en/of verkoop zonder toestemming van de auteur is nadrukkelijk niet toegestaan.
Aan de werking van software en/of hardware kunnen geen rechten worden ontleend. Bouw en gebruik is nadrukkelijk op eigen risico. De seriële poort van de ESP32 staat op de printplaat aangeduid met : G=Ground, T=TX, R=RX.

##Voeding

Dit weerstation wordt gevoed via een USB-C connector. Werkspanning is 5V. Het weerstation verbruikt maximaal 250mA. 

##Bouwinstructie

Ik heb dit project dusdanig ontworpen dat dit ook door niet ervaren zelfbouwers kan worden gebouwd. Het enige onderdeel waarmee je goed op moet letten is de ESP32 processor. Deze is namelijk  in SMD uitgevoerd. Begin altijd met het solderen van één aansluiting in een hoek. Je hebt dan namelijk de mogelijkheid om de ESP32 eventueel nog verder uit te lijnen. Weet je zeker dat de ESP32 goed is uitgelijnd, soldeer dan de andere hoek. Controleer nogmaals of de ESP32 goed is uitgelijnd en vlak op de printplaat ligt, daarna kun je de andere pootjes vast solderen.
Plaats nu vervolgens de overige onderdelen, begin bij laag (weerstanden) en eindig met hoog. Let bij het plaatsen van de transistor, de buzzer en elektrolytische condensatoren op de polariteit.
De bijgeleverde aansluitkabels van de DFRobot modules zijn vrij lang. Deze kun je naar eigen inzicht in te korten.
De aansluitingen van de kleuren van de draden van display en DFRobot modules staan aangegeven met de eerste letter van de kleur. (bv. R = Rood, B = Blauw, Z = zwart etc). Bij de bliksemdetector moet AUX worden aangesloten op de IRQ pin van het detector board. Dit is een losse pen! Bij de LED staat de L voor het lange pootje van de LED.
 
##Onderdelenlijst
Hieronder een overzicht van de benodigde componenten met componentennummer een aantal.

5x	C1, C2, C3, C5, C7		100n condensator (5mm pitch)
1x	C4				            100µF elektrolytische condensator (6.5mm diameter)
1x	C6				            10µF elektrolytische condensator (5.5mm diameter)
3x	R1, R2, R4			      10kΩ 0.25W weerstand
2x	R6, R7				        5.1kΩ 0.25W weerstand
1x	R3				            1kΩ 0.25W weerstand
1x	R5				            470Ω 0.25W weerstand
1x	D1				            3mm LED
1x	U1				            ESP32-WROOM-32UE
1x	U2				            DHT22 module
1x	U3				            LM1117T-3.3
1x	F1				            Zelfherstellende zekering PPTC Polyfuse 500mA (5mm pitch)
1x	SW1				            PCB Drukknop
1x	BZ1				            Actieve buzzer
1x	Q1				            BC547 transistor
1x	J1				            Pinheader, 3-pins (2.54mm pitch)
1x	J2				            ATGM336H GPS Module
1x	J3				            Pinheader, 4-pins (2.54mm pitch)
1x	J4				            DFRobot Gravity SGP40 module
1x	J5				            DFRobot Gravity Bliksem Afstandssensor
1x	J6				            USB-C 2.0 Female naar DIP Adapter
1x					              WiFi 2.4GHz antenne met uFL connector
1x					              Nextion Discovery 3.5” display
5x					              Bout M3 10mm
5x					              Moer M3

Alle bovenstaande onderdelen zijn verkrijgbaar bij Tinytronics.nl
