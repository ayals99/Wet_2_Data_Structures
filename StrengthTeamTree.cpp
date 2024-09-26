#include "StrengthTeamTree.h"

/** Public functions **/
StrengthTeamTree::StrengthTeamTree() : m_size(ZERO), m_root(nullptr){}

StrengthTeamTree::~StrengthTeamTree() {
    deleteTree(m_root);
    m_root = nullptr;
}

bool firstNodeIsStronger(StrengthTeamNode* node1, StrengthTeamNode* node2){
    if (node1 == nullptr || node2 == nullptr){
        return false;
    }

    Team* firstTeam = node1->getTeam(); // O(1)
    Team* secondTeam = node2->getTeam(); // O(1)
    int firstTeamStrength = firstTeam->getStrength(); // O(1)
    int secondTeamStrength = secondTeam->getStrength(); // O(1)

    if (firstTeamStrength > secondTeamStrength){ // O(1)
        return true;
    }
    else if (firstTeamStrength < secondTeamStrength){
        return false;
    }

    else{ // from here on firstTeamStrength == secondTeamStrength
        int firstTeamID = firstTeam->getID(); // O(1)
        int secondTeamID = secondTeam->getID(); // O(1)

        if (firstTeamID < secondTeamID) { // O(1)
            return true;
        }
        return false;
    }
}

bool firstNodeIsStronger(StrengthTeamNode* node1, Team* secondTeam){
    if (node1 == nullptr || secondTeam == nullptr){
        return false;
    }

    Team* firstTeam = node1->getTeam(); // O(1)

    int firstTeamStrength = firstTeam->getStrength(); // O(1)
    int secondTeamStrength = secondTeam->getStrength(); // O(1)

    if (firstTeamStrength > secondTeamStrength){ // O(1)
        return true;
    }
    else if (firstTeamStrength < secondTeamStrength){
        return false;
    }

    else{ // from here on firstTeamStrength == secondTeamStrength
        int firstTeamID = firstTeam->getID(); // O(1)
        int secondTeamID = secondTeam->getID(); // O(1)

        if (firstTeamID < secondTeamID) { // O(1)
            return true;
        }
        return false;
    }
}

int StrengthTeamTree::getNumWins(Team* team) {
     AUX_findTeamIndex(m_root, ZERO, team); // O(log n)
     // AUX_findTeamIndex updates the number of wins field of the team along the way
     return team->getNumOfWins(); // O(1)
}

StrengthTeamNode* StrengthTeamTree::balance_Node(StrengthTeamNode* root){

    if(root->getBalanceFactor() == LEFT_IMBALANCE){
            if(root->getLeft()->getBalanceFactor() >= ZERO){
                return rightRotation(root);
            }
            else{
                return LR_Rotation(root);
            }
        }

        if(root->getBalanceFactor() == RIGHT_IMBALANCE){
            if(root->getRight()->getBalanceFactor() <= ZERO){
                return leftRotation(root);
            }
            else{
                return RL_Rotation(root);
            }
        }
        root->updateMaxRankInSubTree();
        root->updateSizeOfSubTree();
        root->updateHeight();
        return root;
}

StrengthTeamNode* StrengthTeamTree::AUX_insert(StrengthTeamNode* parent, StrengthTeamNode* currentNode, StrengthTeamNode* newNode) {
    currentNode->pushAccumulateToChildren();

    // if newNode is larger than currentNode, we go left
    if ( firstNodeIsStronger(currentNode, newNode) ){
        // if currentNode has no left child, we can insert newNode as its left child
        if (currentNode->getLeft() == nullptr) {
            currentNode->setLeftChild(newNode);
        }
            // if currentNode has a left child, we need to continue the recursion leftwards
        else {
            currentNode->setLeftChild(AUX_insert(currentNode, currentNode->getLeft(), newNode));
        }
    }
    // if newNode is larger than currentNode, we go right
    else{
        // if currentNode has no right child, we can insert newNode as its right child
        if (currentNode->getRight() == nullptr) {
            currentNode->setRightChild(newNode);
        }
            // if currentNode has a right child, we need to continue the recursion rightwards
        else {
            currentNode->setRightChild(AUX_insert(currentNode, currentNode->getRight(), newNode));
        }
    }

    // balance the tree and return the new root of the subtree
    currentNode = balance_Node(currentNode);

    // if currentNode is NOT the root of the tree, we need to tell its parent that his children have changed
    if (parent != nullptr){
        if ( firstNodeIsStronger(parent,newNode) ){
            parent->setLeftChild(currentNode);
        }
        else{
            parent->setRightChild(currentNode);
        }
    }

    // notice that if currentNode is the root of the tree, we don't need to tell anyone in this scope that the root has changed
    return currentNode;
}

void StrengthTeamTree::insert(Team* team) {
    if (team->getStrength() == ZERO || team->getNumPlayers() == ZERO){
        return;
    }

    if (m_root == nullptr){
        m_root = new StrengthTeamNode(team);
        m_size++;
        return;
    }

    StrengthTeamNode* newNode = new StrengthTeamNode(team);
    m_root = AUX_insert(nullptr, m_root, newNode);
    m_size++;
}

StrengthTeamNode* AUX_find_Minimum_In_Subtree(StrengthTeamNode* currentNode){

    if (currentNode == nullptr){
        return nullptr;
    }
    currentNode->pushAccumulateToChildren();
    while(currentNode->getLeft() != nullptr){
        currentNode->pushAccumulateToChildren();
        currentNode = currentNode->getLeft();
        currentNode->pushAccumulateToChildren();
    }
    return currentNode;
}

void StrengthTeamTree::swapData(StrengthTeamNode* firstNode, StrengthTeamNode* secondNode){
    if (firstNode == nullptr || secondNode == nullptr){
        return;
    }
    Team* temp = firstNode->getTeam();
    firstNode->setTeam(secondNode->getTeam());
    secondNode->setTeam(temp);
    balance_Node(firstNode);
}

StrengthTeamNode* StrengthTeamTree::AUX_remove(StrengthTeamNode* currentNode, Team* teamToRemove) {
    // Will only enter this function if the tree holds teamToRemove

    if (currentNode == nullptr){ // if we didn't find the node we want to remove
        return nullptr;
    }

    currentNode->pushAccumulateToChildren();

    // currentNode is not null, we need to see if we arrived at the node we want to remove
    if ( currentNode->getTeam()->getID() == teamToRemove->getID() ){
        // if the data is equal. This means we've found our node to delete.
        if (currentNode->isLeaf()){
            delete currentNode;
            currentNode = nullptr;
            return nullptr;
        }
        else if (currentNode->hasLeftChildOnly()){
            StrengthTeamNode* replacement = currentNode->getLeft();
            delete currentNode;
            currentNode = nullptr;
            return replacement;
        }
        else if (currentNode->hasRightChildOnly()){
            StrengthTeamNode* replacement = currentNode->getRight();
            delete currentNode;
            currentNode = nullptr;
            return replacement;
        }
        else{ // has two children
            // find the smallest value in right subtree
            StrengthTeamNode* temp = AUX_find_Minimum_In_Subtree(currentNode->getRight());
            swapData(currentNode, temp);
            currentNode->setRightChild(AUX_remove(currentNode->getRight(), temp->getTeam()));
        }
    }
    else if ( !firstNodeIsStronger(currentNode, teamToRemove)) {
        // need to search in the right subtree
        currentNode->setRightChild(AUX_remove(currentNode->getRight(), teamToRemove));
    }
    else{ // this means: if firstNodeIsStronger(currentNode, teamToRemove)) == true
        // need to search in the left subtree
        currentNode->setLeftChild(AUX_remove(currentNode->getLeft(), teamToRemove));
    }

    return balance_Node(currentNode);
}

bool StrengthTeamTree::exists_In_Tree(Team* team) { // O(log n)
    if (m_root == nullptr){
        return false;
    }
    StrengthTeamNode* currentNode = m_root;
    while (currentNode != nullptr){

        currentNode->pushAccumulateToChildren(); // O(1)

        Team* currentTeam = currentNode->getTeam(); // O(1)

        if (currentTeam != nullptr){
            if (currentTeam->getID() == team->getID()){
                return true;
            }
        }

        if ( firstNodeIsStronger(currentNode, team) ){ // O(1)
            currentNode = currentNode->getLeft();
        }

        else if ( !firstNodeIsStronger(currentNode, team) ){ // O(1)s
            currentNode = currentNode->getRight();
        }
    }
    return false;
}

void StrengthTeamTree::remove(Team* teamToRemove) {
    if ( !exists_In_Tree(teamToRemove)){
        return;
    }
    m_root = AUX_remove(m_root, teamToRemove);
    m_size--;
}

int StrengthTeamTree::findTeamIndex(Team *team) {
    return AUX_findTeamIndex(m_root, ZERO, team);
}

int StrengthTeamTree::AUX_findTeamIndex(StrengthTeamNode* currentNode, int sumOfIndexesUntilNow, Team* team) {
    //this function assumes that the team is in the tree
    currentNode->pushAccumulateToChildren();

    // if we found the team we were looking for:
    if (currentNode->getTeam()->getID() == team->getID()){
        // we need to add the index of the current node we are at:
        int leftSubtreeSize = (currentNode->getLeft() != nullptr) ? currentNode->getLeft()->getSizeOfSubtree() : 0;
        return sumOfIndexesUntilNow + leftSubtreeSize + ONE; // +ONE for the current node itself.
    }

    if ( firstNodeIsStronger(currentNode, team) ) {
        return AUX_findTeamIndex(currentNode->getLeft(), sumOfIndexesUntilNow, team);
    }
    else { // if currentNode is not the team we're looking for and it's not stronger than the team we're looking for
            int leftSubtreeSize = (currentNode->getLeft() != nullptr) ? currentNode->getLeft()->getSizeOfSubtree() : 0;
            sumOfIndexesUntilNow += leftSubtreeSize + ONE; // +ONE for the current node itself.
            return AUX_findTeamIndex(currentNode->getRight(), sumOfIndexesUntilNow, team);
    }
}



Team* StrengthTeamTree::findTeamByIndex(int indexToSearch) {
    if (indexToSearch > m_size || indexToSearch < ONE){
        return nullptr;
    }
    return AUX_findTeamByIndex(m_root, ONE, indexToSearch);
}

//Changed how the function works, it did not work as intended before
Team* StrengthTeamTree::AUX_findTeamByIndex(StrengthTeamNode* currentNode, int sumOfIndexesUntilNow, int indexToSearch) {
    if (currentNode == nullptr) {
        return nullptr;
    }
    if (currentNode->getLeft() != nullptr) {
        int leftSubtreeSize = currentNode->getLeft()->getSizeOfSubtree();
        int currentNodeIndex = sumOfIndexesUntilNow + leftSubtreeSize; // Current node's index calculation

        if (indexToSearch < currentNodeIndex) {
            return AUX_findTeamByIndex(currentNode->getLeft(), sumOfIndexesUntilNow, indexToSearch);
        } else if (indexToSearch > currentNodeIndex) {
            sumOfIndexesUntilNow += leftSubtreeSize + ONE; // +1 for the current node
            return AUX_findTeamByIndex(currentNode->getRight(), sumOfIndexesUntilNow, indexToSearch);
        } else { // indexToSearch == currentNodeIndex
            return currentNode->getTeam();
        }
    } else {
        // If there is no left child, this means the current node is the next in the sequence
        if (sumOfIndexesUntilNow == indexToSearch) {
            return currentNode->getTeam();
        } else {
            // Adjust sumOfIndexesUntilNow considering the current node
            sumOfIndexesUntilNow += ONE;
            return AUX_findTeamByIndex(currentNode->getRight(), sumOfIndexesUntilNow, indexToSearch);
        }
    }
}

Team* StrengthTeamTree::findCorrespondingLowPowerTeam(int lowPower) {
    Team* team = nullptr;
    StrengthTeamNode* currentNode = m_root;
    while (currentNode != nullptr){
        currentNode->pushAccumulateToChildren();
        Team* currentTeam = currentNode->getTeam();
        int currentTeamStrength = currentTeam->getStrength();

        if (lowPower > currentTeamStrength){
           currentNode = currentNode->getRight();
        }
        else{ // lowPower <= currentTeamStrength
            team = currentTeam;
            currentNode = currentNode->getLeft();
        }
    }
    return team;
}

Team* StrengthTeamTree::findCorrespondingHighPowerTeam(int highPower) {
    Team* team = nullptr;
    StrengthTeamNode* currentNode = m_root;
    while (currentNode != nullptr){
        currentNode->pushAccumulateToChildren();
        Team* currentTeam = currentNode->getTeam();
        int currentTeamStrength = currentTeam->getStrength();

        if (highPower >= currentTeamStrength){
            team = currentTeam;
            currentNode = currentNode->getRight();
        }
        else{ // highPower < currentTeamStrength
            currentNode = currentNode->getLeft();
        }
    }
    return team;
}

void StrengthTeamTree::accumulateWinnings(Team* teamMidBracket, Team* teamHighBracket) {
    if (teamMidBracket == nullptr || teamHighBracket == nullptr){
        return;
    }
    accumulate(teamHighBracket);
    deccumulate(teamMidBracket);
}

void turnRight(StrengthTeamNode* currentNode, bool& RightTurnStreak, bool increment){
    if (currentNode == nullptr){
        return;
    }
    if ( !RightTurnStreak ){
        if (increment){
            currentNode->incrementAccumulate();
        }
        else{
            currentNode->decrementAccumulate();
        }
    }
    RightTurnStreak = true;
}

void turnLeft(StrengthTeamNode* currentNode, bool& RightTurnStreak, bool increment){
    if (currentNode == nullptr){
        return;
    }
    if (RightTurnStreak) {
        if (increment){
            currentNode->decrementAccumulate();
        }
        else{
            currentNode->incrementAccumulate();
        }
    }
    RightTurnStreak = false;
}

/** Private functions **/
void AUX_accumulate(StrengthTeamNode* currentNode, Team* teamHighBracket, bool& RightTurnStreak){
    if (currentNode == nullptr || teamHighBracket == nullptr){
            return;
    }

    currentNode->pushAccumulateToChildren();

    if (currentNode->getTeam()->getID() == teamHighBracket->getID()){
        turnRight(currentNode, RightTurnStreak, true);
        currentNode->pushAccumulateToChildren();
        turnLeft(currentNode->getRight(), RightTurnStreak, true);
    }
    else{
        if (firstNodeIsStronger(currentNode, teamHighBracket)) {
            turnLeft(currentNode, RightTurnStreak, true);
            currentNode->pushAccumulateToChildren();
            AUX_accumulate(currentNode->getLeft(), teamHighBracket, RightTurnStreak);
        }
        else {
            turnRight(currentNode, RightTurnStreak, true);
            currentNode->pushAccumulateToChildren();
            AUX_accumulate(currentNode->getRight(), teamHighBracket, RightTurnStreak);
        }
    }
    currentNode->pushAccumulateToChildren();
    currentNode->updateMaxRankInSubTree();
}

void StrengthTeamTree::accumulate(Team* teamHighBracket) {
    if (teamHighBracket == nullptr){
        return;
    }
    bool RightTurnStreak = false;
    AUX_accumulate(m_root, teamHighBracket, RightTurnStreak);
}

void AUX_deccumulate(StrengthTeamNode* currentNode, Team* teamLowBracket, bool& RightTurnStreak){
    if (currentNode == nullptr || teamLowBracket == nullptr){
        return;
    }

    currentNode->pushAccumulateToChildren();

    if (currentNode->getTeam()->getID() == teamLowBracket->getID()){
        turnLeft(currentNode, RightTurnStreak, false);
        currentNode->pushAccumulateToChildren();
        turnRight(currentNode->getLeft(), RightTurnStreak, false);
    }
    else{
        if (firstNodeIsStronger(currentNode, teamLowBracket)) {
            turnLeft(currentNode, RightTurnStreak, false);
            currentNode->pushAccumulateToChildren();
            AUX_deccumulate(currentNode->getLeft(), teamLowBracket, RightTurnStreak);
        }
        else {
            turnRight(currentNode, RightTurnStreak, false);
            currentNode->pushAccumulateToChildren();
            AUX_deccumulate(currentNode->getRight(), teamLowBracket, RightTurnStreak);
        }
    }
    currentNode->pushAccumulateToChildren();
    currentNode->updateMaxRankInSubTree();
}

void StrengthTeamTree::deccumulate(Team* teamLowBracket) {
    if (teamLowBracket == nullptr){
        return;
    }
    bool RightTurnStreak = false;
    AUX_deccumulate(m_root, teamLowBracket, RightTurnStreak);
}

StrengthTeamNode* StrengthTeamTree::rightRotation(StrengthTeamNode* originalRoot){
    originalRoot->pushAccumulateToChildren();
    StrengthTeamNode* newRoot = originalRoot->getLeft();
    newRoot->pushAccumulateToChildren();
    originalRoot->setLeftChild(newRoot->getRight());
    newRoot->setRightChild(originalRoot);


    originalRoot->updateSizeOfSubTree();
    newRoot->updateSizeOfSubTree();


    originalRoot->updateHeight();
    newRoot->updateHeight();

    originalRoot->updateMaxRankInSubTree();
    newRoot->updateMaxRankInSubTree();
    return newRoot;
}

StrengthTeamNode* StrengthTeamTree::leftRotation(StrengthTeamNode* originalRoot){
    originalRoot->pushAccumulateToChildren();
    StrengthTeamNode* newRoot = originalRoot->getRight();
    newRoot->pushAccumulateToChildren();
    originalRoot->setRightChild(newRoot->getLeft());
    newRoot->setLeftChild(originalRoot);

    originalRoot->updateSizeOfSubTree();
    newRoot->updateSizeOfSubTree();

    originalRoot->updateHeight();
    newRoot->updateHeight();

    originalRoot->updateMaxRankInSubTree();
    newRoot->updateMaxRankInSubTree();

    return newRoot;
}

StrengthTeamNode* StrengthTeamTree::LR_Rotation(StrengthTeamNode* originalRoot){
    originalRoot->setLeftChild( leftRotation(originalRoot->getLeft()) );
    return rightRotation(originalRoot);
}

StrengthTeamNode* StrengthTeamTree::RL_Rotation(StrengthTeamNode* originalRoot){
    originalRoot->setRightChild( rightRotation(originalRoot->getRight()) );
    return leftRotation(originalRoot);
}

void StrengthTeamTree::deleteTree(StrengthTeamNode* node) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->getLeft());
    deleteTree(node->getRight());
    delete node;
    node = nullptr;
}

int StrengthTeamTree::getMaxRankInTree() const { // O(1)
    if (m_root == nullptr){
        return ZERO;
    }
    return m_root->getMaxRankInSubTree(); // O(1)
}
