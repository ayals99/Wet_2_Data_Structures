#include "PlayerTree.h"

/** Public Functions **/
PlayerTree::PlayerTree() {
    m_root = nullptr;
    m_amountOfPlayers = ZERO;
}

PlayerTree::~PlayerTree() {
    deleteTree(m_root);
}

/** Private Functions **/

PlayerTreeNode* PlayerTree::rightRotation(PlayerTreeNode* originalRoot){
    PlayerTreeNode* newRoot = originalRoot->getLeft();
    originalRoot->setLeftChild(newRoot->getRight());
    newRoot->setRightChild(originalRoot);

    originalRoot->updateSizeOfSubTree();
    newRoot->updateSizeOfSubTree();

    originalRoot->updateHeight();
    newRoot->updateHeight();

    return newRoot;
}

PlayerTreeNode* PlayerTree::leftRotation(PlayerTreeNode* originalRoot){
    PlayerTreeNode* newRoot = originalRoot->getRight();
    originalRoot->setRightChild(newRoot->getLeft());
    newRoot->setLeftChild(originalRoot);

    originalRoot->updateSizeOfSubTree();
    newRoot->updateSizeOfSubTree();

    originalRoot->updateHeight();
    newRoot->updateHeight();
    return newRoot;
}

PlayerTreeNode* PlayerTree::LR_Rotation(PlayerTreeNode* originalRoot){
    originalRoot->setLeftChild( leftRotation(originalRoot->getLeft()) );
    return rightRotation(originalRoot);
}

PlayerTreeNode* PlayerTree::RL_Rotation(PlayerTreeNode* originalRoot){
    originalRoot->setRightChild( rightRotation(originalRoot->getRight()) );
    return leftRotation(originalRoot);
}

PlayerTreeNode* PlayerTree::balance_Node(PlayerTreeNode* root){
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

    root->updateSizeOfSubTree();
    root->updateHeight();
    return root;
}

void PlayerTree::deleteTree(PlayerTreeNode* node){
    if(node == nullptr){
        return;
    }
    deleteTree(node->getLeft());
    deleteTree(node->getRight());
    delete node;
    node = nullptr;
}

void PlayerTree::setRoot(PlayerTreeNode* newRoot){
    m_root = newRoot;
}

PlayerTreeNode* PlayerTree::AUX_find(PlayerTreeNode* currentNode, int Key) const{
    if(currentNode == nullptr){
        return nullptr;
    }
    if(Key == currentNode->getPlayer()){
        return currentNode;
    }
    else if (Key > currentNode->getPlayer()){
        return AUX_find(currentNode->getRight(), Key);
    }
    else{
        return AUX_find(currentNode->getLeft(), Key);
    }
}


void PlayerTree::AUX_insert(PlayerTreeNode* parent, PlayerTreeNode* currentNode, Player playerToInsert){
    // This function is called when we know that the player is already in the tree
    // we search recursively for the node that has the strength we want to add
    // when we get to the node, we'll increment the amount of duplicates in the node
    if ( currentNode->getPlayer() == playerToInsert){
        currentNode->incrementAmountOfPlayersInNode();
        currentNode->updateSizeOfSubTree();
        return;
    }
    // if playerToInsert is larger than currentNode, we go right
    else if (playerToInsert > currentNode->getPlayer()) {
        AUX_insert(currentNode, currentNode->getRight(), playerToInsert);
    }
        // if playerToInsert is smaller than currentNode, we go left
    else{
        AUX_insert(currentNode, currentNode->getLeft(), playerToInsert);
    }
    // along the search route back from the recursion, we need to update the size of the subtrees:
    currentNode->updateSizeOfSubTree();
}

PlayerTreeNode* PlayerTree::AUX_insert(PlayerTreeNode* parent, PlayerTreeNode* currentNode, PlayerTreeNode* newNode){
    // if newNode is larger than currentNode, we go left
    if ( currentNode->getPlayer() >= newNode->getPlayer()) {
        // if currentNode has no left child, we can insert newNode as its left child
        if (currentNode->getLeft() == nullptr) {
            currentNode->setLeftChild(newNode);
            // need to add +1 to the size of subtree
            currentNode->updateSizeOfSubTree();
        }
            // if currentNode has a left child, we need to continue the recursion leftwards
        else {
            currentNode->setLeftChild(AUX_insert(currentNode, currentNode->getLeft(), newNode));
        }
    }
        // if newNode is smaller than currentNode, we go right
    else{
        // if currentNode has no right child, we can insert newNode as its right child
        if (currentNode->getRight() == nullptr) {
            currentNode->setRightChild(newNode);
            // need to add +1 to the size of subtree
            currentNode->updateSizeOfSubTree();
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
        if ( parent->getPlayer() > newNode->getPlayer()){
            parent->setLeftChild(currentNode);

        }
        else{
            parent->setRightChild(currentNode);
        }
        parent->updateSizeOfSubTree();
    }

    // notice that if currentNode is the root of the tree, we don't need to tell anyone in this scope that the root has changed

    return currentNode;
}

void PlayerTree::insert(Player player) {
    // if data already exists (country/team/contestant are already in tree)
    // then just increment the amount of duplicates
    // and update the size of the subtrees along the way
    if (PlayerIsInTree(player)) {
        AUX_insert(nullptr, m_root, player);
    }
    // if there is no node that has the strength we want to add, we need to insert a new node
    else {
        PlayerTreeNode *newNode = new PlayerTreeNode(player);
        if (m_root == nullptr) {
            m_root = newNode;
        }
        else {
            PlayerTreeNode* newRoot = AUX_insert(nullptr, m_root, newNode);
            // Check if the root needs to be changed:
            if (m_root != newRoot) {
                m_root = newRoot;
            }
        }
    }
    m_amountOfPlayers++;
}

PlayerTreeNode* PlayerTree::AUX_find_Minimum_In_Subtree(PlayerTreeNode* currentNode){
    if (currentNode == nullptr){
        return nullptr;
    }
    while(currentNode->getLeft() != nullptr){
        currentNode = currentNode->getLeft();
    }
    return currentNode;
}

void swapData(PlayerTreeNode* firstNode, PlayerTreeNode* secondNode){
    if (firstNode == nullptr || secondNode == nullptr){
        return;
    }

    Player temp = firstNode->getPlayer();
    int tempAmount = firstNode->getAmountOfPlayersInNode();

    firstNode->setData(secondNode->getPlayer());
    firstNode->setAmountOfPlayersInNode(secondNode->getAmountOfPlayersInNode());

    secondNode->setData(temp);
    secondNode->setAmountOfPlayersInNode(tempAmount);
}

PlayerTreeNode* PlayerTree::AUX_remove(PlayerTreeNode* currentNode, PlayerTreeNode* nodeToRemove) {
    // Will only enter this function if the tree holds "dataToRemove"
    if (currentNode == nullptr){ // if we didn't find the node we want to remove
        return nullptr;
    }
    // currentNode is not null, we need to see if we arrived at the node we want to remove
    if ( nodeToRemove->getPlayer() == currentNode->getPlayer()){
        // if the data is equal. This means we've found our node to delete.
        if (currentNode->isLeaf()){
            delete currentNode;
            currentNode = nullptr;
            return nullptr;
        }
        else if (currentNode->hasLeftChildOnly()){
            PlayerTreeNode* replacement = currentNode->getLeft();
            delete currentNode;
            currentNode = nullptr;
            return replacement;
        }
        else if (currentNode->hasRightChildOnly()){
            PlayerTreeNode* replacement = currentNode->getRight();
            delete currentNode;
            currentNode = nullptr;
            return replacement;
        }
        else{ // has two children
            // find the smallest value in right subtree
            PlayerTreeNode* temp = AUX_find_Minimum_In_Subtree(currentNode->getRight());
            swapData(currentNode, temp);
            currentNode->setRightChild(AUX_remove(currentNode->getRight(), temp));
        }
    }
    else if ( nodeToRemove->getPlayer() >= currentNode->getPlayer()) {
        // need to search in the right subtree
        currentNode->setRightChild(AUX_remove(currentNode->getRight(), nodeToRemove));
    }
    else{
        // need to search in the left subtree
        currentNode->setLeftChild(AUX_remove(currentNode->getLeft(), nodeToRemove));
    }

    return balance_Node(currentNode);
}

void PlayerTree::AUX_remove(PlayerTreeNode* currentNode, Player playerToRemove){
    // this function is called when we know that the player is already in the tree
    // and the amount of Players in the node is larger than 1
    // therefor, we don't need to delete the node
    if (currentNode->getPlayer() == playerToRemove){
        currentNode->decrementAmountOfPlayersInNode();
        currentNode->updateSizeOfSubTree();
        return;
    }
    else if (playerToRemove > currentNode->getPlayer()){
        AUX_remove(currentNode->getRight(), playerToRemove);
    }
    else{
        AUX_remove(currentNode->getLeft(), playerToRemove);
    }
    currentNode->updateSizeOfSubTree();
}

void PlayerTree::remove(Player player) {
    PlayerTreeNode* nodeWithPlayer = AUX_find(m_root, player);
    if (nodeWithPlayer == nullptr){
        return;
    }
    // if there are duplicates of the player, we just decrement the amount of duplicates
    // and update the size of the subtrees along the way
    if (nodeWithPlayer->getAmountOfPlayersInNode() > ONE){
        AUX_remove(m_root, player);
    }
    // if there is only one player in the node, we need to remove the node
    else {
        m_root = AUX_remove(m_root, nodeWithPlayer);
    }
    m_amountOfPlayers--;
}

int divideByTwoAndFloor(int number){
    return number / TWO;
}

// findMedian can't be called on an empty team
Player PlayerTree::findMedian() {
    // NOTICE: player.index - ONE = amount of players that he is stronger than

    // define the median:
    // the player who is stronger than floor(amountOfPlayers/2) weakest players in Team.
    // for example, if we have 1 player, the median is the player himself because he is stronger than floor(1/2) = 0 players
    // if we have 2 players, the median is the player who is stronger than floor(2/2) = 1 player, therefore it is the player with index 2

    int median = divideByTwoAndFloor(m_amountOfPlayers) + ONE; // Notice integer division will use floor()
    return findPlayerByRank(m_root, ZERO, median);
}





int getLowestRankInSubtree(PlayerTreeNode* root, int sumOfRanksUntilNow){
    int rightSubtreeSize = root->getRight() == nullptr ? ZERO : root->getRight()->getSizeOfSubTree();
    int amountOfPlayersInNode = root->getAmountOfPlayersInNode();

    return sumOfRanksUntilNow + root->getSizeOfSubTree() - amountOfPlayersInNode - rightSubtreeSize  + ONE;
}

Player PlayerTree::findPlayerByRank(PlayerTreeNode* root, int sumOfRanksUntilNow, int rankToSearch) {
    int lowestRankInNode = getLowestRankInSubtree(root, sumOfRanksUntilNow);
    int greatestRankInNode = lowestRankInNode + (root->getAmountOfPlayersInNode() - ONE);

    if ( (lowestRankInNode <= rankToSearch) && (rankToSearch <= greatestRankInNode) ){
        return root->getPlayer();
    }
    // turn left:
    else if (rankToSearch < lowestRankInNode){
        return findPlayerByRank(root->getLeft(), sumOfRanksUntilNow, rankToSearch);
    }

    else {
        sumOfRanksUntilNow = greatestRankInNode;
        return findPlayerByRank(root->getRight(), greatestRankInNode, rankToSearch);
    }

}

bool PlayerTree::PlayerIsInTree(Player player) {
    return (bool) AUX_find(m_root, player);
}

void PlayerTree::setSize(int newSize) {
    m_amountOfPlayers = newSize;
}

PlayerTreeNode* PlayerTree::getRoot() const {
    return m_root;
}

Player* AUX_inOrderToArray(PlayerTreeNode* root, Player* array, int* index)
{
    if (root == nullptr){
        return array;
    }

    AUX_inOrderToArray(root->getLeft(), array, index);
    // add the player to the array, while there are still players in the array
    int amountOfPlayersInNode = root->getAmountOfPlayersInNode();
    while (amountOfPlayersInNode > ZERO){
        array[*index] = root->getPlayer();
        amountOfPlayersInNode--;
        (*index)++;
    }

    AUX_inOrderToArray(root->getRight(), array, index);

    return array;
}

// we assume that that the array is of length m_amountOfPlayers
void PlayerTree::InOrderToArray(Player* array) {
        if (this->m_amountOfPlayers == ZERO){
            return;
        }
        int* index = new int(ZERO);
        AUX_inOrderToArray(m_root, array, index);
        delete index;
}

int PlayerTree::getAmountOfPlayers() const{
        return m_amountOfPlayers;
}







