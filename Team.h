#ifndef WET2_DATA_STRUCTURES_TEAM_H
#define WET2_DATA_STRUCTURES_TEAM_H

#include "PlayerTree.h"
#include "arrivalQueue.h"
#include <iostream>
#include <cmath>

class Team{
private:
    int m_teamId;
    int m_numOfPlayers;
    int m_strength;
    int m_numOfWins;
    PlayerTree* m_players;
    arrivalQueue* m_arrivalQueue;

public:
    /**The Holy Trinity**/
    Team(int teamId);
    Team(const Team& team) = delete;
    ~Team();

    /**Getters & Setters**/
    int getID() const;
    int getNumOfWins() const;
    int getNumPlayers() const;
    int getStrength() const;

    void changeNumOfWins(int change);
    void decrementNumberOfPlayers();
    void incrementNumberOfPlayers();

    /**Utility Functions**/
    void insert(int player);
    void removeNewestPlayer(); // O(log(k))
    void addArrivalQueueToStart(Team* otherTeam); // O(k_otherTeam)
    void updateStrength(); // O(log(k))

    void mergeTeams(Team* otherTeam);

    /**Operators**/
    bool operator==(const Team& other) const;
    bool operator>=(const Team& other) const;


//    friend std::ostream &operator<<(std::ostream &os, const Team &team);
};


#endif //WET2_DATA_STRUCTURES_TEAM_H
