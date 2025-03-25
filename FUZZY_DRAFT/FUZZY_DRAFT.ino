#include <Arduino.h>
#include <avr/pgmspace.h>
#include <TimeLib.h>  // For epoch conversion

// Data structure matching your CSV
typedef struct {
    uint32_t epoch;     // Seconds since 1970-01-01
    bool tideType;      // 1=High, 0=Low
    bool isConsecutive; // 1=true, 0=false
} TideEntry;

// Your complete dataset (truncated for example)
const TideEntry tideSchedule[] PROGMEM = {
    // Epoch timestamp | Tide (1=High/0=Low) | isConsecutive (from Logic)
    {1740869340, 1, 1}, {1740959100, 0, 0}, {1741004400, 0, 0}, {1741092120, 0, 0},
    {1741180140, 0, 0}, {1741268700, 0, 0}, {1741358100, 1, 1}, {1741449600, 1, 1},
    {1741542300, 1, 1}, {1741633560, 1, 1}, {1741723920, 1, 1}, {1741813680, 1, 1},
    {1741902960, 1, 1}, {1741992060, 1, 1}, {1742080980, 0, 0}, {1742167020, 0, 0},
    {1742210100, 0, 0}, {1742296980, 0, 0}, {1742384640, 0, 0}, {1742472900, 0, 0},
    {1742561640, 1, 1}, {1742651100, 1, 1}, {1742742000, 1, 1}, {1742834820, 1, 1},
    {1742926080, 1, 1}, {1743016320, 1, 1}, {1743106140, 1, 1}, {1743195900, 1, 1},
    {1743285780, 1, 1}, {1743375960, 0, 0}, {1743418560, 0, 0}, {1743506340, 1, 1},
    {1743594540, 1, 1}, {1743683100, 1, 1}, {1743772140, 1, 1}, {1743862140, 1, 1},
    {1743953940, 1, 1}, {1744046280, 1, 1}, {1744137840, 1, 1}, {1744228560, 0, 0},
    {1744318620, 0, 0}, {1744408200, 0, 0}, {1744497480, 0, 0}, {1744538940, 0, 0},
    {1744625460, 1, 1}, {1744712340, 1, 1}, {1744799940, 1, 1}, {1744888020, 1, 1},
    {1744976580, 1, 1}, {1745065560, 1, 1}, {1745155080, 1, 1}, {1745245440, 1, 1},
    {1745337000, 1, 1}, {1745428380, 1, 1}, {1745519460, 1, 1}, {1745610660, 0, 0},
    {1745701920, 0, 0}, {1745746020, 1, 1}, {1745833500, 1, 1}, {1745921460, 1, 1},
    {1746009780, 1, 1}, {1746098460, 1, 1}, {1746187560, 1, 1}, {1746277140, 1, 1},
    {1746367380, 1, 1}, {1746458340, 1, 1}, {1746549540, 0, 0}, {1746641160, 0, 0},
    {1746732780, 0, 0}, {1746781080, 0, 0}, {1746867720, 0, 0}, {1746954600, 1, 1},
    {1747041420, 1, 1}, {1747128420, 1, 1}, {1747215960, 1, 1}, {1747304400, 1, 1},
    {1747390380, 1, 1}, {1747481160, 1, 1}, {1747570260, 1, 1}, {1747659660, 1, 1},
    {1747749480, 1, 1}, {1747839780, 1, 1}, {1747930680, 1, 1}, {1748023020, 0, 0},
    {1748074020, 0, 0}, {1748161140, 1, 1}, {1748248860, 1, 1}, {1748337000, 1, 1},
    {1748425560, 1, 1}, {1748514420, 1, 1}, {1748603580, 1, 1}, {1748693040, 1, 1},
    {1748782800, 1, 1}, {1748872560, 1, 1}, {1748962200, 1, 1}, {1749052080, 0, 0},
    {1749111240, 0, 0}, {1749195960, 0, 0}, {1749282780, 1, 1}, {1749369960, 1, 1},
    {1749457260, 1, 1}, {1749544740, 1, 1}, {1749632460, 1, 1}, {1749720480, 1, 1},
    {1749808800, 1, 1}, {1749897360, 1, 1}, {1749986220, 1, 1}, {1750075260, 1, 1},
    {1750164540, 1, 1}, {1750253940, 1, 1}, {1750344240, 1, 1}, {1750433940, 0, 0},
    {1750488000, 1, 1}, {1750575660, 1, 1}, {1750663980, 1, 1}, {1750752600, 1, 1},
    {1750841520, 1, 1}, {1750930680, 1, 1}, {1751019960, 1, 1}, {1751109360, 1, 1},
    {1751198820, 1, 1}, {1751288220, 1, 1}, {1751377200, 1, 1}, {1751465940, 0, 0},
    {1751554740, 0, 0}, {1751608440, 0, 0}, {1751695920, 0, 0}, {1751783940, 1, 1},
    {1751872260, 1, 1}, {1751960520, 1, 1}, {1752048840, 1, 1}, {1752137100, 1, 1},
    {1752225480, 1, 1}, {1752313920, 1, 1}, {1752402600, 1, 1}, {1752491460, 1, 1},
    {1752580500, 1, 1}, {1752669720, 1, 1}, {1752759120, 1, 1}, {1752848640, 0, 0},
    {1752899520, 1, 1}, {1752988320, 1, 1}, {1753077960, 1, 1}, {1753167720, 1, 1},
    {1753257360, 1, 1}, {1753346880, 1, 1}, {1753436340, 1, 1}, {1753525800, 1, 1},
    {1753615140, 1, 1}, {1753704360, 1, 1}, {1753793280, 1, 1}, {1753881900, 0, 0},
    {1753970400, 0, 0}, {1754016480, 0, 0}, {1754104020, 0, 0}, {1754193180, 1, 1},
    {1754283900, 1, 1}, {1754374560, 1, 1}, {1754464200, 1, 1}, {1754553240, 1, 1},
    {1754641920, 1, 1}, {1754730480, 1, 1}, {1754819100, 1, 1}, {1754907840, 1, 1},
    {1754996940, 1, 1}, {1755086220, 1, 1}, {1755175740, 1, 1}, {1755222120, 0, 0},
    {1755310260, 1, 1}, {1755399240, 1, 1}, {1755489600, 1, 1}, {1755581280, 1, 1},
    {1755672300, 1, 1}, {1755762660, 1, 1}, {1755852540, 1, 1}, {1755942120, 1, 1},
    {1756031400, 1, 1}, {1756120620, 1, 1}, {1756209600, 1, 1}, {1756298520, 0, 0},
    {1756384920, 0, 0}, {1756470780, 1, 1}, {1756558440, 1, 1}, {1756603620, 1, 1},
    {1756693920, 1, 1}, {1756784040, 1, 1}, {1756876740, 1, 1}, {1756967580, 1, 1},
    {1757057220, 1, 1}, {1757146320, 1, 1}, {1757235240, 1, 1}, {1757324160, 1, 1},
    {1757413440, 1, 1}, {1757503020, 1, 1}, {1757592960, 1, 1}, {1757678400, 1, 1},
    {1757766720, 1, 1}, {1757812260, 1, 1}, {1757901660, 1, 1}, {1757992860, 1, 1},
    {1758085560, 1, 1}, {1758177180, 1, 1}, {1758267900, 1, 1}, {1758357900, 1, 1},
    {1758447540, 1, 1}, {1758536880, 1, 1}, {1758626040, 1, 1}, {1758754620, 1, 1},
    {1758841680, 1, 1}, {1758929280, 1, 1}, {1759017420, 1, 1}, {1759052220, 0, 0},
    {1759145640, 1, 1}, {1759195320, 1, 1}, {1759285920, 1, 1}, {1759378500, 1, 1},
    {1759470060, 1, 1}, {1759560420, 1, 1}, {1759650300, 1, 1}, {1759740060, 1, 1},
    {1759829880, 1, 1}, {1759962000, 1, 1}, {1760049900, 1, 1}, {1760138280, 1, 1},
    {1760226960, 1, 1}, {1760316120, 1, 1}, {1760406060, 1, 1}, {1760497620, 1, 1},
    {1760589900, 1, 1}, {1760681520, 1, 1}, {1760772660, 1, 1}, {1760863140, 1, 1},
    {1760996400, 1, 1}, {1761083280, 1, 1}, {1761170160, 1, 1}, {1761257220, 1, 1},
    {1761344700, 1, 1}, {1761432720, 1, 1}, {1761521100, 1, 1}, {1761556980, 0, 0},
    {1761609960, 1, 1}, {1761699240, 1, 1}, {1761789240, 1, 1}, {1761880320, 1, 1},
    {1761971760, 1, 1}, {1762063020, 1, 1}, {1762202940, 1, 1}, {1762289940, 1, 1},
    {1762377480, 1, 1}, {1762465440, 1, 1}, {1762553820, 1, 1}, {1762642620, 1, 1},
    {1762731720, 1, 1}, {1762821360, 1, 1}, {1762911660, 1, 1}, {1763002500, 1, 1},
    {1763093520, 1, 1}, {1763185140, 0, 0}, {1763325120, 0, 0}, {1763411940, 1, 1},
    {1763499060, 1, 1}, {1763586240, 1, 1}, {1763673480, 1, 1}, {1763761080, 1, 1},
    {1763848980, 1, 1}, {1763937180, 1, 1}, {1764022140, 1, 1}, {1764111180, 1, 1},
    {1764203760, 1, 1}, {1764293220, 1, 1}, {1764383040, 1, 1}, {1764473460, 1, 1}
};
const int tideScheduleSize = sizeof(tideSchedule) / sizeof(TideEntry);

// Helper to read PROGMEM data
TideEntry getTideData(int index) {
    TideEntry entry;
    memcpy_P(&entry, &tideSchedule[index], sizeof(TideEntry));
    return entry;
}

// Convert epoch to readable date/time (UTC)
String epochToDateTime(uint32_t epoch) {
    char buf[20];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d",
             year(epoch), month(epoch), day(epoch),
             hour(epoch), minute(epoch));
    return String(buf);
}

void setup() {
    Serial.begin(115200);
    
    // Example: Print first 5 entries with converted dates
    for (int i = 0; i < min(272, tideScheduleSize); i++) {
        TideEntry e = getTideData(i);
        Serial.print(epochToDateTime(e.epoch));
        Serial.print(" | ");
        Serial.print(e.tideType ? "High" : "Low");
        Serial.print(e.isConsecutive ? " (Consec)" : "");
        Serial.println();
    }
}

void loop() {}