#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>

int lowTideAlertLevel = 0; 

RTC_DS3231 rtc;

// Data structure for tide schedule
typedef struct {
    uint32_t epoch;     // Time in seconds
    bool tideType;      // 1=High, 0=Low
} TideEntry;

const TideEntry tideSchedule[] PROGMEM = {
    {1740869340, 1}, {1740959100, 0}, {1741004400, 0}, {1741092120, 0},
    {1741180140, 0}, {1741268700, 0}, {1741358100, 1}, {1741449600, 1},
    {1741542300, 1}, {1741633560, 1}, {1741723920, 1}, {1741813680, 1},
    {1741902960, 1}, {1741992060, 1}, {1742080980, 0}, {1742167020, 0},
    {1742210100, 0}, {1742296980, 0}, {1742384640, 0}, {1742472900, 0},
    {1742561640, 1}, {1742651100, 1}, {1742742000, 1}, {1742834820, 1},
    {1742926080, 1}, {1743016320, 1}, {1743106140, 1}, {1743195900, 1},
    {1743285780, 1}, {1743375960, 0}, {1743418560, 0}, {1743506340, 1},
    {1743594540, 1}, {1743683100, 1}, {1743772140, 1}, {1743862140, 1},
    {1743953940, 1}, {1744046280, 1}, {1744137840, 1}, {1744228560, 0},
    {1744318620, 0}, {1744408200, 0}, {1744497480, 0}, {1744538940, 0},
    {1744625460, 1}, {1744712340, 1}, {1744799940, 1}, {1744888020, 1},
    {1744976580, 1}, {1745065560, 1}, {1745155080, 1}, {1745245440, 1},
    {1745337000, 1}, {1745428380, 1}, {1745519460, 1}, {1745610660, 0},
    {1745701920, 0}, {1745746020, 1}, {1745833500, 1}, {1745921460, 1},
    {1746009780, 1}, {1746098460, 1}, {1746187560, 1}, {1746277140, 1},
    {1746367380, 1}, {1746458340, 1}, {1746549540, 0}, {1746641160, 0},
    {1746732780, 0}, {1746781080, 0}, {1746867720, 0}, {1746954600, 1},
    {1747041420, 1}, {1747128420, 1}, {1747215960, 1}, {1747390380, 1},
    {1747481160, 1}, {1747570260, 1}, {1747659660, 1}, {1747749480, 1},
    {1747839780, 1}, {1747930680, 1}, {1748023020, 0}, {1748074020, 0},
    {1748161140, 1}, {1748248860, 1}, {1748337000, 1}, {1748425560, 1},
    {1748514420, 1}, {1748603580, 1}, {1748693040, 1}, {1748782800, 1},
    {1748872560, 1}, {1748962200, 1}, {1749052080, 0}, {1749111240, 0},
    {1749195960, 0}, {1749282780, 1}, {1749369960, 1}, {1749457260, 1},
    {1749544740, 1}, {1749632460, 1}, {1749720480, 1}, {1749808800, 1},
    {1749897360, 1}, {1749986220, 1}, {1750075260, 1}, {1750164540, 1},
    {1750253940, 1}, {1750344240, 1}, {1750433940, 0}, {1750488000, 1},
    {1750575660, 1}, {1750663980, 1}, {1750752600, 1}, {1750841520, 1},
    {1750930680, 1}, {1751019960, 1}, {1751109360, 1}, {1751198820, 1},
    {1751288220, 1}, {1751377200, 1}, {1751465940, 0}, {1751554740, 0},
    {1751608440, 0}, {1751695920, 0}, {1751783940, 1}, {1751872260, 1},
    {1751960520, 1}, {1752048840, 1}, {1752137100, 1}, {1752225480, 1},
    {1752313920, 1}, {1752402600, 1}, {1752491460, 1}, {1752580500, 1},
    {1752669720, 1}, {1752759120, 1}, {1752848640, 0}, {1752899520, 1},
    {1752988320, 1}, {1753077960, 1}, {1753167720, 1}, {1753257360, 1},
    {1753346880, 1}, {1753436340, 1}, {1753525800, 1}, {1753615140, 1},
    {1753704360, 1}, {1753793280, 1}, {1753881900, 0}, {1753970400, 0},
    {1754016480, 0}, {1754104020, 0}, {1754193180, 1}, {1754283900, 1},
    {1754374560, 1}, {1754464200, 1}, {1754553240, 1}, {1754641920, 1},
    {1754730480, 1}, {1754819100, 1}, {1754907840, 1}, {1754996940, 1},
    {1755086220, 1}, {1755175740, 1}, {1755222120, 0}, {1755310260, 1},
    {1755399240, 1}, {1755489600, 1}, {1755581280, 1}, {1755672300, 1},
    {1755762660, 1}, {1755852540, 1}, {1755942120, 1}, {1756031400, 1},
    {1756120620, 1}, {1756209600, 1}, {1756298520, 0}, {1756384920, 0},
    {1756470780, 1}, {1756558440, 1}, {1756603620, 1}, {1756693920, 1},
    {1756784040, 1}, {1756876740, 1}, {1756967580, 1}, {1757057220, 1},
    {1757146320, 1}, {1757235240, 1}, {1757324160, 1}, {1757413440, 1},
    {1757503020, 1}, {1757592960, 1}, {1757678400, 1}, {1757766720, 1},
    {1757812260, 1}, {1757901660, 1}, {1757992860, 1}, {1758085560, 1},
    {1758177180, 1}, {1758267900, 1}, {1758357900, 1}, {1758447540, 1},
    {1758536880, 1}, {1758626040, 1}, {1758754620, 1}, {1758841680, 1}

};

// Function to extract date components from an epoch timestamp
void getDateFromEpoch(uint32_t epoch, int &year, int &month, int &day) {
    DateTime dt = DateTime(epoch);
    year = dt.year();
    month = dt.month();
    day = dt.day();
}

// Function to check for all tide events within the current day
void checkFutureLowTides(uint32_t currentEpoch) {
    size_t startIdx = 0;
    bool foundStart = false;

    // Find the first future tide event (from today onward)
    for (size_t i = 0; i < sizeof(tideSchedule) / sizeof(TideEntry); i++) {
        TideEntry entry;
        memcpy_P(&entry, &tideSchedule[i], sizeof(TideEntry));

        if (entry.epoch >= currentEpoch) {
            startIdx = i;
            foundStart = true;
            break;
        }
    }

    if (!foundStart) {
        Serial.println("No future tide data available.");
        return;
    }

    Serial.println("Checking consecutive low tides from today onward...");

    // Scan forward in the tide dataset
    int lowTideCount = 0; 
    const size_t scanLimit = 6;
    size_t scanCounter = 0; 

    for (size_t i = startIdx; i < sizeof(tideSchedule) / sizeof(TideEntry); i++) {
        if (scanCounter >= scanLimit) break;

        TideEntry entry;
        memcpy_P(&entry, &tideSchedule[i], sizeof(TideEntry));

        if (entry.tideType == 0) {
        } else { 
            lowTideCount = 0; 
        }

        // Print each detected tide
        Serial.print("Epoch: ");
        Serial.print(entry.epoch);
        Serial.print(" | Tide: ");
        Serial.println(entry.tideType ? "High" : "Low");

        // Trigger alerts
        if (lowTideCount == 3) {
            Serial.println("Alert: 3 consecutive low tides detected!");
            lowTideAlertLevel = 3;
        } else if (lowTideCount == 5) {
            Serial.println("Alert: 5 or more consecutive low tides detected!");
            lowTideAlertLevel = 5;
            break; 
        }

        scanCounter++; 
    }


    // Serial.print("Final Consecutive Low Tides Count: ");
    // Serial.println(lowTideCount);
}



void setup() {
    Serial.begin(115200);
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    // Get current time
    DateTime now = rtc.now();
    uint32_t currentEpoch = now.unixtime();

    // Process today's tide events
    checkFutureLowTides(currentEpoch);
    Serial.print("lowtideCount: ");
    Serial.println(lowTideAlertLevel);
}


void loop() {
    // Nothing here for now
}
