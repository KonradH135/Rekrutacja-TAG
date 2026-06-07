/*
Data:    18.01.2025    
Zadanie: Przelewanka
Autor:   Konrad Hałaszko
Indeks:  479234
*/
#include<bits/stdc++.h>

using namespace std;

/**
 * Custom Hash Functor for std::vector<int>.
 * * Standard C++ does not provide a default hash function for vectors.
 * Since we want to use 'vector<int>' (the state of cups) as a key 
 * in an 'unordered_map', we must define how to hash it.
 * original source: Boost library
 */
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t seed = 0;
        for (int i : v) {
            // Combines the hash of the current integer with the existing seed.
            // This specific bitwise magic (0x9e3779b9 + shifts) is derived from 
            // Boost library's hash_combine to minimize collisions.
            seed ^= hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

void read_data(int &number_of_cups, vector<int> &maximal_values, vector<int> &expected_values){
    // Read total number of cups
    if(scanf("%d",&number_of_cups) != 1){
        fprintf(stderr,"ERROR: Failed to read the number of cups\n");
        exit(1);
    }    

    // Resize vectors to hold data for each cup
    maximal_values.resize(number_of_cups);
    expected_values.resize(number_of_cups);

    // Read capacity and target value for each cup pair
    for(int i = 0; i < number_of_cups; ++i){
        if(scanf("%d %d", &maximal_values[i], &expected_values[i]) != 2){
            fprintf(stderr,"ERROR: Failed to read sizes of cups\n");
            exit(1);
        }
    }
}

bool solution_cannot_exists(const int &number_of_cups, const vector<int> &maximal_values , const vector<int> &expected_values){
    bool expected_to_be_empty = false, expected_to_be_full = false;

    // Check basic constraints on the target values
    for(int i = 0; i < number_of_cups; ++i){
        if(maximal_values[i] == expected_values[i]){
            expected_to_be_full = true;
        }
        if(expected_values[i] == 0){
            expected_to_be_empty = true;
        }
    }

    // Heuristic 1: If the goal requires NO cups to be completely full and NO cups to be 
    // completely empty (intermediate values only), assume impossible based on problem constraints.
    if(!expected_to_be_empty && !expected_to_be_full){
        return true;
    }

    // Heuristic 2: Number Theory.
    // The amount of water in any cup is always a linear combination of cup capacities.
    // Therefore, any reachable amount must be a multiple of the GCD (Greatest Common Divisor)
    // of all cup capacities.
    int gcd_of_maximal_values = maximal_values[0];
    for(int i = 1; i < number_of_cups; ++i){
        gcd_of_maximal_values = __gcd(gcd_of_maximal_values, maximal_values[i]);
    }
    for(int i = 0; i < number_of_cups; ++i){

        // If any expected value is not divisible by the GCD, the state is unreachable.
        if(expected_values[i]%gcd_of_maximal_values != 0){
            return true;
        }
    }
    return false;
}

/**
 * Checks for trivial solutions where no pouring between cups is required.
 * This happens if the target state consists only of cups that are either 
 * fully empty or fully filled to their maximum capacity.
 */
bool trivial_cases(const int &number_of_cups, const vector<int> &maximal_values, const vector<int> &expected_values){
    bool solution = true;
    int counter = 0;
    for(int i = 0; i < number_of_cups; ++i){
        // If target is neither 0 nor max_capacity, it's not a trivial case
        if((expected_values[i] != 0) && (expected_values[i] != maximal_values[i])){
            solution = false;
        }

        // Count how many operations (fills) are at least needed
        if(expected_values[i] > 0){
            ++counter;
        }
    }
    if(solution){
        // Print the number of "Fill" operations needed
        printf("%d\n",counter);
    }
    return solution;
}

/**
 * Validates if the current state of cups matches the expected target state.
 */
bool are_values_correct(const int &number_of_cups, const vector<int> &current_values, const vector<int> &expected_values){
    bool answer = true;
    int index = 0;

    // Iterate through all cups to check if current level == target level
    while(answer && (index < number_of_cups)){
        answer = answer && (current_values[index] == expected_values[index]);
        ++index;
    }
    return answer;
}

/**
 * Analyzes a newly generated state (configuration).
 * 1. Checks if it has been visited before.
 * 2. Checks if it is the solution (if so, prints result and exits).
 * 3. Adds it to the BFS queue and marks as visited.
 */
void analize_configuration(
    const int &number_of_cups,
    const int &depth,
    const vector<int> &current_values,
    const vector<int> &expected_values,
    queue<int> &depths,
    queue<vector<int>> &reachable_state,
    unordered_map<vector<int>, bool, VectorHash> &visited_states){

    // Only process if this state hasn't been processed yet (duplicate state pruning)
    if(!visited_states.count(current_values)){

        // Check if we found the solution
        if(are_values_correct(number_of_cups, current_values, expected_values)){
            // Print the number of steps and terminate the program immediately
            printf("%d\n",depth);
            exit(0);
        }

        // Mark as visited to avoid cycles and redundant processing
        visited_states[current_values] = true;

        // Push state and its depth to the BFS queues
        depths.push(depth);
        reachable_state.push(current_values);
    }
}

/**
 * Generates all possible next states (neighbors) from the current state.
 * Possible moves:
 * 1. Empty a cup.
 * 2. Fill a cup.
 * 3. Pour water from cup A to cup B.
 */
void process_state(
    const int &number_of_cups,
    const int &depth, 
    vector<int> &current_values, 
    const vector<int> &maximal_values, 
    const vector<int> &expected_values, 
    queue<int> &depths,
    queue<vector<int>> &reachable_state,
    unordered_map<vector<int>, bool, VectorHash> &visited_states){

    // --- Operation Type 1 & 2: Emptying and Filling ---
    for(int i = 0; i < number_of_cups; ++i){

        // Store original value to restore later
        int current_value = current_values[i];

        // Try Emptying cup i
        current_values[i] = 0;
        analize_configuration(number_of_cups, depth, current_values, expected_values, depths, reachable_state, visited_states);

        // Try Filling cup i (restore to max capacity)
        current_values[i] = maximal_values[i];
        analize_configuration(number_of_cups, depth, current_values, expected_values, depths, reachable_state, visited_states);

        // Backtracking step: Restore the original value of cup i for the next iteration
        current_values[i] = current_value;
    }

    // --- Operation Type 3: Pouring between cups ---
    for(int i = 0; i < number_of_cups; ++i){
        for(int j = i + 1; j < number_of_cups; ++j){
            // We save copies because we modify current_values in place
            int copies_of_current_value[2];
            copies_of_current_value[0] = current_values[i];
            copies_of_current_value[1] = current_values[j];
            
            // --- Case A: Pour from cup 'i' to cup 'j' ---
            // New amount in 'j' is min(capacity of j, sum of i + j)
            current_values[j] = min(maximal_values[j], current_values[j] + current_values[i]);
            // Amount left in 'i' = (Previous i + Previous j) - New j
            current_values[i] = current_values[i] - current_values[j] + copies_of_current_value[1];

            analize_configuration(number_of_cups, depth, current_values, expected_values, depths, reachable_state, visited_states);

            // Restore state before trying the reverse pour
            current_values[i] = copies_of_current_value[0];
            current_values[j] = copies_of_current_value[1];

            // --- Case B: Pour from cup 'j' to cup 'i' ---
            // New amount in 'i' is min(capacity of i, sum of i + j)
            current_values[i] = min(maximal_values[i], current_values[j] + current_values[i]);
            // Amount left in 'j' = (Previous i + Previous j) - New i
            current_values[j] = current_values[j] - current_values[i] + copies_of_current_value[0];

            analize_configuration(number_of_cups, depth, current_values, expected_values, depths, reachable_state, visited_states);

            // Restore state for the next loop iteration
            current_values[i] = copies_of_current_value[0];
            current_values[j] = copies_of_current_value[1];

        }
    }
}

/**
 * Main BFS (Breadth-First Search) driver.
 * Initializes the queues and processes states layer by layer to find the shortest path.
 */
void solve(const int &number_of_cups, const vector<int> &maximal_values, const vector<int> &expected_values){
    // Queue for BFS states
    queue<vector<int>> reachable_state;
    // Map to keep track of visited states and avoid infinite loops
    unordered_map<vector<int>, bool, VectorHash> visited_states;
    // Queue for tracking the depth (number of moves) corresponding to states
    queue<int> depths;

    // Initialize BFS with the starting state (depth 0, all cups empty)
    depths.push(0);
    vector<int> current_values;
    current_values.resize(number_of_cups); // Default initialization to 0s
    reachable_state.push(current_values);
    visited_states[current_values] = true;

    // BFS Loop
    while(!reachable_state.empty()){
        // Pop the front state
        current_values = reachable_state.front();
        reachable_state.pop();

        int depth = depths.front();
        depths.pop();

        // Generate and process all neighbors for this state (depth + 1)
        process_state(number_of_cups, depth + 1, current_values, maximal_values, expected_values, depths, reachable_state, visited_states);
    }
}


int main(){
    int number_of_cups;
    vector<int> maximal_values, expected_values;

    // 1. Read input
    read_data(number_of_cups, maximal_values, expected_values);

    // 2. Check for trivial scenarios (direct fills)
    if(trivial_cases(number_of_cups, maximal_values, expected_values)){
        return 0;
    }

    // 3. Check for impossible scenarios
    if(solution_cannot_exists(number_of_cups, maximal_values, expected_values)){
        printf("-1\n");
        return 0;
    }

    // 4. Run BFS solver
    solve(number_of_cups, maximal_values, expected_values);

    // 5. If queue is empty and no solution found
    printf("-1\n");
    return 0;
}