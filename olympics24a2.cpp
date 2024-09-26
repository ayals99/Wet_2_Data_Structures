#include "olympics24a2.h"


olympics_t::olympics_t()
{
    try {
        m_teamHashTable = new TeamTreeHashTable();
    } catch (std::bad_alloc& e) {
        throw e;
    }
    try {
        m_strengthTeamTree = new StrengthTeamTree();
    }catch (std::bad_alloc& e) {
        delete m_teamHashTable;
        m_teamHashTable = nullptr;
        throw e;
    }
}

olympics_t::~olympics_t()
{
    delete m_teamHashTable;
    delete m_strengthTeamTree;
    m_teamHashTable = nullptr;
    m_strengthTeamTree = nullptr;
}


StatusType olympics_t::add_team(int teamId){

    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (m_teamHashTable->member(teamId)) { //O(1) amortized
        return StatusType::FAILURE;
    }
    try {
        Team* team = new Team(teamId);
        m_teamHashTable->insert(team); //O(1) amortized
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_team(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_teamHashTable->member(teamId)) { //O(1) amortized
        return StatusType::FAILURE;
    }
    try {
        Team* team = m_teamHashTable->find(teamId); //O(1) amortized
        m_strengthTeamTree->remove(team); //O(log(n))
        m_teamHashTable->remove(team);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId, int playerStrength)
{

    if (teamId <= 0 || playerStrength <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_teamHashTable->member(teamId)) {
        return StatusType::FAILURE;
    }
    try {
        Team* team = m_teamHashTable->find(teamId); //O(1) amortized
        if (!team->getNumPlayers())
        {
            team->insert(playerStrength); //O(log(k))
            m_strengthTeamTree->insert(team); //O(log(n))
        }
        else{
            m_strengthTeamTree->remove(team); //O(log(n))
            team->insert(playerStrength); //O(log(k))
            m_strengthTeamTree->insert(team); //O(log(n))
        }
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_teamHashTable->member(teamId)) {
        return StatusType::FAILURE;
    }
    try {
        Team* team = m_teamHashTable->find(teamId); //O(1) amortized
        if (team->getNumPlayers() == 0) {
            return StatusType::FAILURE;
        }
        m_strengthTeamTree->remove(team); //O(log(n))
        team->removeNewestPlayer(); //O(log(k))
        if (team->getNumPlayers() > 0) {
            m_strengthTeamTree->insert(team); //O(log(n))
        }
    }
    catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

void updateHighestRank(Team* team1, Team* team2, StrengthTeamTree* strengthTeamTree) {
    if (team1 == nullptr || team2 == nullptr) {
        return;
    }
    strengthTeamTree->remove(team1); //O(log(n))
    strengthTeamTree->remove(team2); //O(log(n))
    strengthTeamTree->insert(team1); //O(log(n))
    strengthTeamTree->insert(team2); //O(log(n))
}


output_t<int> olympics_t::play_match(int teamId1, int teamId2){
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_teamHashTable->member(teamId1) || !m_teamHashTable->member(teamId2)) { //O(1) amortized
        return StatusType::FAILURE;
    }
    try {
        Team* team1 = m_teamHashTable->find(teamId1); //O(1) amortized
        Team* team2 = m_teamHashTable->find(teamId2); //O(1) amortized
        if (team1->getNumPlayers() == 0 || team2->getNumPlayers() == 0) {
            return StatusType::FAILURE;
        }
        int strength1 = team1->getStrength(); //O(1)
        int strength2 = team2->getStrength(); //O(1)
        int id1 = team1->getID(); //O(1)
        int id2 = team2->getID(); //O(1)
        if (strength1 > strength2) {
            team1->changeNumOfWins(ONE); //O(1)
            updateHighestRank(team1, team2, m_strengthTeamTree); //O(log(n)
            return teamId1;
        }
        else if (strength1 < strength2){
            team2->changeNumOfWins(ONE); //O(1)
            updateHighestRank(team1, team2, m_strengthTeamTree); //O(log(n)
            return teamId2;
        }
        else{
            //if the strengths are equal, the team with the smaller id wins
            if (id1 < id2) {
                team1->changeNumOfWins(ONE); //O(1)
                updateHighestRank(team1, team2, m_strengthTeamTree); //O(log(n)
                return teamId1;
            } else {
                team2->changeNumOfWins(ONE); //O(1)
                updateHighestRank(team1, team2, m_strengthTeamTree); //O(log(n)
                return teamId2;
            }
        }
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_teamHashTable->member(teamId)) { //O(1) amortized
        return StatusType::FAILURE;
    }
    try {
        Team* team = m_teamHashTable->find(teamId); //O(1) amortized
        // if the team is empty, it is not in the strength tree and then we'll return the number of wins
        if (team->getNumPlayers() == ZERO ){
            return team->getNumOfWins(); //O(1)
        }
        return m_strengthTeamTree->getNumWins(team); //O(log(n))
    }
    catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> olympics_t::get_highest_ranked_team(){
    try
    {
        if (m_teamHashTable->isEmpty()) { //O(1)
            return EMPTY_MAX_RANK;
        }
        return m_strengthTeamTree->getMaxRankInTree(); //O(1)
    }
    catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{


    if (teamId1 <= ZERO || teamId2 <= ZERO || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_teamHashTable->member(teamId1) || !m_teamHashTable->member(teamId2)) { //O(1) amortized
        return StatusType::FAILURE;
    }
    try {
        Team* team1 = m_teamHashTable->find(teamId1); //O(1) amortized
        Team* team2 = m_teamHashTable->find(teamId2); //O(1) amortized

        m_strengthTeamTree->remove(team1); // O(log(n))
        m_strengthTeamTree->remove(team2); // O(log(n))

        team1->mergeTeams(team2); // O(k)
        // delete team2 from the system.
        // note that the hash table is responsible for deleting the team:
        m_teamHashTable->remove(team2); //O(1) amortized

        m_strengthTeamTree->insert(team1); //O(log(n))
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

bool isPowerOfTwo(int n) {
    return (n > 0) && ( (n & (n - 1)) == 0);
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    if (lowPower <= 0 || highPower <= 0 || lowPower >= highPower) {
        return StatusType::INVALID_INPUT;
    }
    try {
        Team* teamLowBracket = m_strengthTeamTree->findCorrespondingLowPowerTeam(lowPower); //O(log(n))
        Team* teamHighBracket = m_strengthTeamTree->findCorrespondingHighPowerTeam(highPower); //O(log(n))

        if (teamLowBracket == nullptr || teamHighBracket == nullptr) {
            return StatusType::FAILURE;
        }

        int teamLowBracketIndex = m_strengthTeamTree->findTeamIndex(teamLowBracket); //O(log(n))
        int teamHighBracketIndex = m_strengthTeamTree->findTeamIndex(teamHighBracket); //O(log(n))

        int numTeams = teamHighBracketIndex - teamLowBracketIndex + ONE;

        if (numTeams == ONE) {
            return teamHighBracket->getID(); //O(1)
        }

        if (numTeams < TWO || !isPowerOfTwo(numTeams)) {
                return StatusType::FAILURE;
        }

        while (numTeams > ONE){ // O(log i) iterations, each iteration costs O(log n), total of O( log n * log i)
            int mid_bracket_index = teamLowBracketIndex + (numTeams / TWO);
            Team* teamMidBracket = m_strengthTeamTree->findTeamByIndex(mid_bracket_index); //O(log(n))
            m_strengthTeamTree->accumulateWinnings(teamMidBracket, teamHighBracket); // O(log(n)
             numTeams = numTeams / TWO;
             teamLowBracketIndex = mid_bracket_index;
        }

        return teamHighBracket->getID(); //O(1)

    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

}
