#include <Arduino.h>

// ==================== SYSTEM CONFIGURATION ====================
#define MAX_CANDIDATES 5      // Maximum number of candidates
#define MAX_VOTERS 10         // Maximum number of voters
#define MAX_NAME_LENGTH 20    // Maximum candidate name length

// ==================== DATA STRUCTURES ====================
struct Candidate {
    int id;
    char name[MAX_NAME_LENGTH + 1];
    int voteCount;
};

struct Voter {
    int id;
    bool hasVoted;
};

// ==================== GLOBAL VARIABLES ====================
Candidate candidates[MAX_CANDIDATES];
Voter voters[MAX_VOTERS];
int candidateCount = 0;
int voterCount = 0;
int totalVotes = 0;

// ==================== FUNCTION PROTOTYPES ====================
void mainMenu();
void registerCandidate();
void registerVoter();
void castVote();
void displayResults();
void displayCandidates();
void saveData();
void loadData();
void initializeDemoData();
void waitForEnter();
void printHeader();
String readString();

// ==================== SETUP FUNCTION ====================
void setup() {
    Serial.begin(9600);
    
    // Give time for Serial Monitor to open
    for(int i = 0; i < 10 && !Serial; i++) {
        delay(100);
    }
    
    printHeader();
    initializeDemoData();
    
    Serial.println(F("\nSystem ready! Press Enter to continue..."));
    waitForEnter();
    mainMenu();
}

// ==================== LOOP FUNCTION ====================
void loop() {
    // All processing happens in mainMenu
}

// ==================== UTILITY FUNCTIONS ====================
void printHeader() {
    Serial.println(F("\n========================================"));
    Serial.println(F("       ARDUINO VOTING SYSTEM"));
    Serial.println(F("          PlatformIO Version"));
    Serial.println(F("========================================"));
}

void waitForEnter() {
    while (Serial.available()) Serial.read(); // Clear buffer
    while (!Serial.available()); // Wait for input
    while (Serial.available()) Serial.read(); // Clear after
}

String readString() {
    String str = "";
    while (true) {
        if (Serial.available()) {
            char c = Serial.read();
            if (c == '\n' || c == '\r') {
                break;
            }
            str += c;
        }
    }
    return str;
}

// ==================== INITIALIZE DEMO DATA ====================
void initializeDemoData() {
    Serial.println(F("\nInitializing demo data..."));
    
    // Add default candidates using Arduino-compatible syntax
    strcpy(candidates[0].name, "Alice Johnson");
    candidates[0].id = 1;
    candidates[0].voteCount = 0;
    
    strcpy(candidates[1].name, "Bob Smith");
    candidates[1].id = 2;
    candidates[1].voteCount = 0;
    
    strcpy(candidates[2].name, "Carol Davis");
    candidates[2].id = 3;
    candidates[2].voteCount = 0;
    
    candidateCount = 3;
    
    // Add default voters
    for (int i = 0; i < 5; i++) {
        voters[i].id = 101 + i;
        voters[i].hasVoted = false;
        voterCount++;
    }
    
    Serial.println(F("✓ Demo data initialized"));
    Serial.println(F("✓ 3 candidates added"));
    Serial.println(F("✓ 5 voters registered"));
}

// ==================== MAIN MENU ====================
void mainMenu() {
    while (true) {
        Serial.println(F("\n=== VOTING SYSTEM MENU ==="));
        Serial.println(F("==========================="));
        Serial.println(F("1. Register New Candidate"));
        Serial.println(F("2. Register New Voter"));
        Serial.println(F("3. View All Candidates"));
        Serial.println(F("4. Cast Vote"));
        Serial.println(F("5. View Election Results"));
        Serial.println(F("6. Save Data (Simulated)"));
        Serial.println(F("7. Load Data (Simulated)"));
        Serial.println(F("8. Clear All Data"));
        Serial.println(F("9. Exit"));
        Serial.println(F("==========================="));
        
        Serial.print(F("\nEnter your choice (1-9): "));
        while (!Serial.available());
        char choice = Serial.read();
        Serial.println(choice);
        
        switch(choice) {
            case '1':
                registerCandidate();
                break;
            case '2':
                registerVoter();
                break;
            case '3':
                displayCandidates();
                break;
            case '4':
                castVote();
                break;
            case '5':
                displayResults();
                break;
            case '6':
                saveData();
                break;
            case '7':
                loadData();
                break;
            case '8':
                candidateCount = 0;
                voterCount = 0;
                totalVotes = 0;
                Serial.println(F("\n✓ All data cleared!"));
                break;
            case '9':
                Serial.println(F("\nThank you for using Voting System!"));
                Serial.println(F("Goodbye!"));
                return;
            default:
                Serial.println(F("Invalid choice! Try again."));
        }
        
        Serial.println(F("\nPress Enter to continue..."));
        waitForEnter();
    }
}

// ==================== REGISTER CANDIDATE ====================
void registerCandidate() {
    if (candidateCount >= MAX_CANDIDATES) {
        Serial.println(F("\nERROR: Maximum candidates reached!"));
        return;
    }
    
    Serial.println(F("\n--- REGISTER NEW CANDIDATE ---"));
    
    // Get ID
    Serial.print(F("Enter candidate ID: "));
    while (!Serial.available());
    int id = Serial.parseInt();
    Serial.println(id);
    
    // Check if ID exists
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].id == id) {
            Serial.println(F("ERROR: ID already exists!"));
            return;
        }
    }
    
    // Clear buffer
    while (Serial.read() != -1);
    
    // Get name
    Serial.print(F("Enter candidate name: "));
    String name = readString();
    Serial.println(name);
    
    // Save candidate
    candidates[candidateCount].id = id;
    name.toCharArray(candidates[candidateCount].name, MAX_NAME_LENGTH);
    candidates[candidateCount].voteCount = 0;
    candidateCount++;
    
    Serial.print(F("\nSUCCESS: Candidate '"));
    Serial.print(name);
    Serial.println(F("' registered!"));
}

// ==================== REGISTER VOTER ====================
void registerVoter() {
    if (voterCount >= MAX_VOTERS) {
        Serial.println(F("\nERROR: Maximum voters reached!"));
        return;
    }
    
    Serial.println(F("\n--- REGISTER NEW VOTER ---"));
    
    Serial.print(F("Enter voter ID: "));
    while (!Serial.available());
    int id = Serial.parseInt();
    Serial.println(id);
    
    // Check if ID exists
    for (int i = 0; i < voterCount; i++) {
        if (voters[i].id == id) {
            Serial.println(F("ERROR: Voter ID already exists!"));
            return;
        }
    }
    
    voters[voterCount].id = id;
    voters[voterCount].hasVoted = false;
    voterCount++;
    
    Serial.print(F("\nSUCCESS: Voter ID "));
    Serial.print(id);
    Serial.println(F(" registered!"));
}

// ==================== DISPLAY CANDIDATES ====================
void displayCandidates() {
    if (candidateCount == 0) {
        Serial.println(F("\nNo candidates registered yet!"));
        return;
    }
    
    Serial.println(F("\n=== REGISTERED CANDIDATES ==="));
    Serial.println(F("ID  Name                  Votes"));
    Serial.println(F("-------------------------------"));
    
    for (int i = 0; i < candidateCount; i++) {
        Serial.print(candidates[i].id);
        if (candidates[i].id < 10) Serial.print("   ");
        else Serial.print("  ");
        
        Serial.print(candidates[i].name);
        
        // Add spacing for alignment
        int nameLength = strlen(candidates[i].name);
        for (int j = nameLength; j < 20; j++) {
            Serial.print(" ");
        }
        
        Serial.println(candidates[i].voteCount);
    }
    
    Serial.println(F("-------------------------------"));
    Serial.print(F("Total candidates: "));
    Serial.println(candidateCount);
}

// ==================== CAST VOTE ====================
void castVote() {
    if (candidateCount == 0) {
        Serial.println(F("\nERROR: No candidates registered!"));
        return;
    }
    
    Serial.println(F("\n=== CAST YOUR VOTE ==="));
    
    // Get voter ID
    Serial.print(F("Enter your voter ID: "));
    while (!Serial.available());
    int voterId = Serial.parseInt();
    Serial.println(voterId);
    
    // Find voter
    int voterIndex = -1;
    for (int i = 0; i < voterCount; i++) {
        if (voters[i].id == voterId) {
            voterIndex = i;
            break;
        }
    }
    
    if (voterIndex == -1) {
        Serial.println(F("ERROR: Voter ID not found!"));
        return;
    }
    
    if (voters[voterIndex].hasVoted) {
        Serial.println(F("ERROR: You have already voted!"));
        return;
    }
    
    // Show candidates
    displayCandidates();
    
    // Get candidate choice
    Serial.print(F("\nEnter candidate ID to vote for: "));
    while (!Serial.available());
    int candidateId = Serial.parseInt();
    Serial.println(candidateId);
    
    // Find candidate
    int candidateIndex = -1;
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].id == candidateId) {
            candidateIndex = i;
            break;
        }
    }
    
    if (candidateIndex == -1) {
        Serial.println(F("ERROR: Invalid candidate ID!"));
        return;
    }
    
    // Cast vote
    candidates[candidateIndex].voteCount++;
    voters[voterIndex].hasVoted = true;
    totalVotes++;
    
    Serial.println(F("\n=== VOTE CAST SUCCESSFULLY! ==="));
    Serial.print(F("You voted for: "));
    Serial.println(candidates[candidateIndex].name);
    Serial.print(F("Total votes cast: "));
    Serial.println(totalVotes);
    Serial.println(F("================================"));
}

// ==================== DISPLAY RESULTS ====================
void displayResults() {
    if (candidateCount == 0) {
        Serial.println(F("\nNo candidates to display results!"));
        return;
    }
    
    if (totalVotes == 0) {
        Serial.println(F("\nNo votes cast yet!"));
        return;
    }
    
    Serial.println(F("\n=== ELECTION RESULTS ==="));
    Serial.println(F("ID  Name                  Votes   %"));
    Serial.println(F("-------------------------------"));
    
    int maxVotes = 0;
    int winnerIndex = -1;
    bool isTie = false;
    
    // First pass: display all candidates
    for (int i = 0; i < candidateCount; i++) {
        float percentage = (candidates[i].voteCount * 100.0) / totalVotes;
        
        Serial.print(candidates[i].id);
        if (candidates[i].id < 10) Serial.print("   ");
        else Serial.print("  ");
        
        Serial.print(candidates[i].name);
        
        int nameLength = strlen(candidates[i].name);
        for (int j = nameLength; j < 20; j++) {
            Serial.print(" ");
        }
        
        Serial.print(candidates[i].voteCount);
        
        // Align percentage column
        if (candidates[i].voteCount < 10) Serial.print("      ");
        else if (candidates[i].voteCount < 100) Serial.print("     ");
        else Serial.print("    ");
        
        Serial.print(percentage, 1);
        Serial.println("%");
        
        // Track winner
        if (candidates[i].voteCount > maxVotes) {
            maxVotes = candidates[i].voteCount;
            winnerIndex = i;
            isTie = false;
        } else if (candidates[i].voteCount == maxVotes && maxVotes > 0) {
            isTie = true;
        }
    }
    
    Serial.println(F("-------------------------------"));
    Serial.print(F("Total votes: "));
    Serial.println(totalVotes);
    
    // Announce winner
    Serial.println(F("\n=== ELECTION OUTCOME ==="));
    if (isTie && maxVotes > 0) {
        Serial.println(F("IT'S A TIE!"));
        Serial.print(F("Candidates with "));
        Serial.print(maxVotes);
        Serial.println(F(" votes:"));
        
        for (int i = 0; i < candidateCount; i++) {
            if (candidates[i].voteCount == maxVotes) {
                Serial.print("  - ");
                Serial.println(candidates[i].name);
            }
        }
    } else if (winnerIndex != -1) {
        float winPercent = (candidates[winnerIndex].voteCount * 100.0) / totalVotes;
        Serial.println(F("WINNER:"));
        Serial.print(F("  "));
        Serial.println(candidates[winnerIndex].name);
        Serial.print(F("  Votes: "));
        Serial.print(candidates[winnerIndex].voteCount);
        Serial.print(F(" ("));
        Serial.print(winPercent, 1);
        Serial.println(F("%)"));
    }
    Serial.println(F("=========================="));
}

// ==================== FILE OPERATIONS (SIMULATED) ====================
void saveData() {
    Serial.println(F("\n--- SAVE DATA ---"));
    Serial.println(F("(Simulated file save)"));
    
    Serial.print(F("✓ Saved "));
    Serial.print(candidateCount);
    Serial.println(F(" candidates"));
    
    Serial.print(F("✓ Saved "));
    Serial.print(voterCount);
    Serial.println(F(" voters"));
    
    Serial.print(F("✓ Saved "));
    Serial.print(totalVotes);
    Serial.println(F(" total votes"));
    
    Serial.println(F("\nData saved successfully!"));
}

void loadData() {
    Serial.println(F("\n--- LOAD DATA ---"));
    Serial.println(F("(Simulated - loading demo data)"));
    
    // Reset and load demo data
    candidateCount = 0;
    voterCount = 0;
    totalVotes = 0;
    
    initializeDemoData();
    
    Serial.println(F("\n✓ Demo data loaded successfully!"));
}