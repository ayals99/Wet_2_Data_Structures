#include "Team.h"
#include "arrivalQueue.h"


Team::Team(int teamId) : m_teamId(teamId),
                        m_numOfPlayers(ZERO),
                        m_strength(ZERO),
                        m_numOfWins(ZERO) {
    try {
        m_players = new PlayerTree();
    }
    catch (std::bad_alloc& e) {
        throw e;
    }
    try {
        m_arrivalQueue = new arrivalQueue();
    }
    catch (std::bad_alloc& e) {
        delete m_players;
        throw e;
    }
}

Team::~Team() {
    delete m_players;
    delete m_arrivalQueue;
}

int Team::getID() const {
    return m_teamId;
}

int Team::getNumOfWins() const {
    return m_numOfWins;
}

int Team::getNumPlayers() const {
    return m_numOfPlayers;
}

int Team::getStrength() const {
    return m_strength;
}

void Team::changeNumOfWins(int change) {
    m_numOfWins += change;
}

void Team::decrementNumberOfPlayers() {
    m_numOfPlayers--;
}

void Team::incrementNumberOfPlayers() {
    m_numOfPlayers++;
}

void Team::insert(int player) {
    // insert the player to the player tree
    m_players->insert(player); // O(log(k))

    // insert the player to the arrival queue
    m_arrivalQueue->push(player); // O(1) amortized

    // update the number of players
    incrementNumberOfPlayers(); // O(1)

    // update the strength
    updateStrength(); // O(log(k))
}

void Team::removeNewestPlayer() {
    // if there are no players in the team, do nothing:
    if (m_numOfPlayers == ZERO) {
        return;
    }

    // find the player to remove
    Player playerToRemove = m_arrivalQueue->top(); // O(1)
    // remove the player from the player tree
    if (playerToRemove == NOT_FOUND){
        return;
    }
    m_players->remove(playerToRemove); // O(log(k))
    // remove the player from the arrival queue

    m_arrivalQueue->pop(); // O(1) amortized
    // update the number of players
    decrementNumberOfPlayers(); // O(1)

    // update the strength
    updateStrength(); //  O(log(k))
}

void Team::addArrivalQueueToStart(Team* otherTeam) {
    if (otherTeam == nullptr) {
        return;
    }
    // add the other team's arrival queue to the start of this team's arrival queue
    m_arrivalQueue->appendToStart(otherTeam->m_arrivalQueue);
}

void Team::updateStrength() {
    if (m_players->getAmountOfPlayers() == 0) {
        m_strength = 0;
        return;
    }

    m_strength = m_players->getAmountOfPlayers() * m_players->findMedian();
}

bool Team::operator==(const Team& other) const {
    return m_teamId == other.m_teamId;
}

bool Team::operator>= (const Team& other) const {
    return m_teamId >= other.m_teamId;
}

PlayerTreeNode* createEmptyNode(){
    PlayerTreeNode* newNode = new PlayerTreeNode(); // O(1)
    return newNode;
}

PlayerTreeNode* AUX_EmptyTree(int height){
    if (height <= 0){ // reached a leaf
        return createEmptyNode();
    }
    PlayerTreeNode* newNode = createEmptyNode();
    newNode->setLeftChild(AUX_EmptyTree(height - 1));
    newNode->setRightChild(AUX_EmptyTree(height - 1));
    newNode->updateHeight();
    return newNode;
}

// This functions receives a full binary tree with height h and turns it into an almost full tree
void deleteSpareNodes(PlayerTree* tree, PlayerTreeNode* parent, PlayerTreeNode* current, int height, int numberOfElements){
    if (tree->getAmountOfPlayers() <= numberOfElements){
        return;
    }
    if (current->isLeaf() && height == 0){
        if(parent != nullptr){
            if (current == parent->getRight()) {
                parent->setRightChild(nullptr);
            }
            else {
                parent->setLeftChild(nullptr);
            }
        }
        tree->setSize(tree->getAmountOfPlayers() - 1);
        delete current;
        current = nullptr;
    }
    else{
        deleteSpareNodes(tree, current, current->getRight(), height - 1, numberOfElements);
        deleteSpareNodes(tree, current, current->getLeft(), height - 1, numberOfElements);
    }
}

PlayerTree* createEmptyTree(int numberOfElements){
    PlayerTree* newTree = new PlayerTree(); // O(1)
    if (numberOfElements == ZERO){
        newTree->setRoot(nullptr);
        newTree->setSize(ZERO);
        return newTree;
    }
    int height = (int)floor(log2(numberOfElements + 1));

    newTree->setRoot(AUX_EmptyTree(height)); // O(k)

    int fullBinaryTreeSize = 0;
    for (int i = height; i >= ZERO; i--){
        fullBinaryTreeSize += (int)pow(2, i);
    }

    newTree->setSize(fullBinaryTreeSize);

    deleteSpareNodes(newTree , nullptr, newTree->getRoot(), height, numberOfElements);// O(k)
    return newTree;
}


void AUX_insertArrayToTreeInOrder(Player* array, int arraySize, PlayerTreeNode* current, int& index){ // O(n)
    if (current == nullptr){
        return;
    }

    AUX_insertArrayToTreeInOrder(array, arraySize, current->getLeft(), index);

    current->setData(array[index]);
    index++;

    while( (index < arraySize) && (array[index] == array[index - ONE])){
        current->incrementAmountOfPlayersInNode();
        index++;
    }
    current->updateSizeOfSubTree(); // O(1)

    AUX_insertArrayToTreeInOrder(array, arraySize, current->getRight(), index);

    current->updateSizeOfSubTree();
}


void insertArrayToTree(Player* array, int arraySize ,PlayerTree* tree){ // O(k)
    int index = 0;
    if (array == nullptr){
        return;
    }
    AUX_insertArrayToTreeInOrder(array, arraySize, tree->getRoot(), index);
}

int countUniqueElements(Player* array, int numberOfElements){ // O(k)
    if (array == nullptr){
        return ZERO;
    }
    int count = 1;
    for (int i = 1; i < numberOfElements; i++) {
        if (array[i] != array[i - 1]) {
            count++;
        }
    }
    return count;
}

PlayerTree* ArrayToTree(Player* array, int numberOfElements){
    int numberOfUniqueElements = countUniqueElements(array, numberOfElements); // O(k)
    PlayerTree* newTree = createEmptyTree(numberOfUniqueElements); // O(k)
    if (array != nullptr) {
        insertArrayToTree(array, numberOfElements, newTree); // O(k)
    }
    newTree->setSize(numberOfElements);
    return newTree;
}

void mergeArrays(Player* mergedIdArray, Player* sortedId1, Player* sortedId2, int length1, int length2){
    if (length1 == ZERO && length2 == ZERO) {
        return;
    }
    // if arrived here, that means that at least one of the arrays is not empty
    int i = 0;
    int j = 0;
    int k = 0;
    if ( (length1 != ZERO)
         && (length2 != ZERO)
         && (sortedId1 != nullptr)
         && (sortedId2 != nullptr)
         && (mergedIdArray != nullptr)){
        while (i < length1 && j < length2) {
            if (sortedId2[j] >= sortedId1[i]) {
                mergedIdArray[k] = sortedId1[i];
                i++;
            } else {
                mergedIdArray[k] = sortedId2[j];
                j++;
            }
            k++;
        }
    }
    if ((length1 != ZERO) && (sortedId1 != nullptr) && (mergedIdArray != nullptr)){
        while (i < length1) {
            mergedIdArray[k] = sortedId1[i];
            i++;
            k++;
        }
    }
    if ((length2 != ZERO) && (sortedId2 != nullptr) && (mergedIdArray != nullptr)){
        while (j < length2) {
            mergedIdArray[k] = sortedId2[j];
            j++;
            k++;
        }
    }
}

void Team::mergeTeams(Team* otherTeam) {
    if (otherTeam == nullptr) {
        return;
    }
    if (otherTeam->m_numOfPlayers == ZERO) {
        return;
    }
    // this function needs to take another team, and merge all the players from the second team
    // into the first team

    // turn first team playerTree into array:
    Player* thisTeamArray = nullptr;
    if (m_numOfPlayers != ZERO) {
        thisTeamArray = new Player[m_numOfPlayers];
    }
    this->m_players->InOrderToArray(thisTeamArray);
    delete this->m_players;
    m_players = nullptr;

    // turn other team playerTree into array:
    Player* otherTeamArray = new Player[otherTeam->m_numOfPlayers];
    otherTeam->m_players->InOrderToArray(otherTeamArray);

    // merge the two arrays:
    int newNumOfPlayers = m_numOfPlayers + otherTeam->m_numOfPlayers;
    Player* mergedArray = new Player[newNumOfPlayers];
    mergeArrays(mergedArray, thisTeamArray, otherTeamArray, m_numOfPlayers, otherTeam->m_numOfPlayers);

    delete [] thisTeamArray;
    delete [] otherTeamArray;

    // create a new playerTree from the merged array and assign it to the first team
    m_players = ArrayToTree(mergedArray, newNumOfPlayers);
    delete [] mergedArray;


    // update strength and amount of players in first team:
    m_numOfPlayers = newNumOfPlayers;
    updateStrength();

    // then merge the arrival queues such that the second team's arrival queue will be at the start
    // of the first team's arrival queue
    this->addArrivalQueueToStart(otherTeam);

}


//std::ostream &operator<<(std::ostream &os, const Team &team) {
//    os << "Team ID: " << team.m_teamId << std::endl;
//    os << "Number of players: " << team.m_numOfPlayers << std::endl;
//    os << "Number of wins: " << team.m_numOfWins << std::endl;
//    os << "Strength: " << team.m_strength << std::endl;
//    os << "----------------------------" << std::endl;
//    return os;
//}


