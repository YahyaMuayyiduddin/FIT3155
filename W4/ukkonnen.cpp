Traversal_Result find_endpoint(std::string& input, int phase, Node* active_node, int suffix_start_index, int j){
        int target = phase - 1;
        int remainder_length = target - suffix_start_index + 1;
        // j == phase
        if (j == phase) {
            return {nullptr, active_node, -1, -1, -1, Traversal_Endpoint::LAST_EXTENSION};
        }


        // No remainder
        if (remainder_length == 0) {
            // Rule 2 alternate, rule 2 root, rule 1
            if (!active_node->has_edge(input[suffix_start_index])){
                return {nullptr, active_node, -1, -1, phase, Traversal_Endpoint::NO_REMAINDER};
            }
            // Rule 3 end of edge
            return {nullptr, active_node, -1, -1, phase, Traversal_Endpoint::NO_REMAINDER_RULE_3};
        }

        int current_pos = suffix_start_index;
        Edge* current_edge = active_node->get_edge(input[current_pos]);
        Node* current_node = active_node;

        int edge_length = (current_edge->get_end() - current_edge->suffix_start + 1);
        int new_remainder = remainder_length - edge_length;
        
        if (new_remainder == 0 ){
            return {current_edge, current_node, current_edge->suffix_start, current_edge->get_end(), target - remainder_length + 1 , Traversal_Endpoint::END_EDGE};
        }
        
        if (new_remainder > 0) {
            throw std::runtime_error("Invariant violated: remainder > 0, suffix links malformed");
        }
        // Mid edge stop, rule 2 regular, or rule 3
        return {current_edge, current_node, current_edge->suffix_start, current_edge->suffix_start + remainder_length - 1, target - remainder_length + 1 , Traversal_Endpoint::MID_EDGE};