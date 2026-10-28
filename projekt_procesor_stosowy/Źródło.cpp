#include <iostream>
#include <stdlib.h> //malloc
using namespace std;

#define MAX_INSTRUCTION_LENGTH 20000

// --- Structure for Nodes in the INNER lists (holding characters) ---
struct CharListNode {
	char data;
	CharListNode* previous;
	CharListNode* next;
};

// --- Structure for the INNER lists (Stacks of characters) ---
struct CharList {
	CharListNode* head; // Top of the character stack
	CharListNode* tail;
};

// --- Structure for Nodes in the MAIN Stack (holding pointers to CharLists) ---
struct MetaListNode {
	CharList* list_data; // Pointer to an inner character list
	MetaListNode* previous;
	MetaListNode* next;
};

// --- Structure for the MAIN Stack (Stack of CharLists) ---
struct MetaList {
	MetaListNode* head; // Top of the stack-of-lists
	MetaListNode* tail;
};

void initializeCharList(CharList* list)
{
	if (list != nullptr) {
		list->head = nullptr;
		list->tail = nullptr;
	}
	list->head = nullptr;
	list->tail = nullptr;
}

//functions to simplify freeing memory
void freeCharListNodes(CharListNode* node) 
{
	if (node == nullptr) 
	{
		return;
	}
	// Recurse to the end of the list
	CharListNode* next_node = node->next;
	freeCharListNodes(next_node);

	free(node);
}


void freeCharList(CharList* list) 
{
	if (list == nullptr) 
	{
		return; //nothing to free
	}
	freeCharListNodes(list->head);
	free(list);
}

void addFrontCharList(CharList* list, char value)
{
	CharListNode* node = (CharListNode*)malloc(sizeof(CharListNode));

	node->data = value;
	node->previous = NULL;
	node->next = list->head;

	//this is to check if the starting list is empty or not
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->head->previous = node;
		list->head = node;
	}
}

void removeTopCharList(MetaList* meta_list)
{
	if (meta_list == nullptr || meta_list->head == nullptr) 
	{
		return; //nothing to remove
	}

	MetaListNode* node_to_remove = meta_list->head;

	meta_list->head = meta_list->head->next;

	if (meta_list->head == nullptr)
	{
		meta_list->tail = nullptr;
	}
	else
	{
		meta_list->head->previous = nullptr;
	}

	//if this pointed to a charlist, free it
	if (node_to_remove->list_data != nullptr)
	{

		freeCharList(node_to_remove->list_data);
	}

	free(node_to_remove);

}

CharListNode* findTailRecursive(CharListNode* current_node) 
{
	if (current_node == nullptr)
	{
		return nullptr;
	}
	//if this node's next is null, then its the tail
	if (current_node->next == nullptr)
	{
		return current_node;
	}
	return findTailRecursive(current_node->next);
}

CharListNode* copyCharNodesRecursive(const CharListNode* source_node, CharListNode** tail_of_copy_output)
{
	//base case: end of the source list
	if (source_node == nullptr) {
		*tail_of_copy_output = nullptr; //no tail for an empty list
		return nullptr; //return null head for an empty list
	}

	// Create and copy the current node first
	CharListNode* new_node = (CharListNode*)malloc(sizeof(CharListNode));
	if (new_node == nullptr)
	{
		cout << "malloc failed in copyCharNodesRecursive" << endl;
		*tail_of_copy_output = nullptr;
		return nullptr;
	}
	new_node->data = source_node->data;
	new_node->previous = nullptr; //will be linked by recursion

	//copy the rest of the list.
	//recursive will fill tail_of_copy_output with the tail of the rest of the list.
	new_node->next = copyCharNodesRecursive(source_node->next, tail_of_copy_output);

	//link the rest back to the current new_node
	if (new_node->next != nullptr) {
		new_node->next->previous = new_node;
		//tail_of_copy_output already holds the tail from the deeper recursion
	}
	else {
		//if new_node->next is null, then this new_node is the last node copied
		//it's the tail of the entire segment being copied by this call stack
		//set the output parameter to this node
		*tail_of_copy_output = new_node;
	}

	//return the head of the copied segment (this new_node)
	return new_node;
}

CharList* duplicateCharListRecursive(const CharList* source_list)
{

	CharList* new_list = (CharList*)malloc(sizeof(CharList));
	if (new_list == nullptr) 
	{
		cout << "malloc failed in duplicateCharListRecursive" << endl;
		return nullptr;
	}

	initializeCharList(new_list); //sets head/tail to nullptr

	//empty source list check
	if (source_list == nullptr || source_list->head == nullptr)
	{
		return new_list; //return empty initialized list
	}

	//variable to receive the tail pointer from the copy function
	CharListNode* new_tail_ptr = nullptr;

	//call the modified recursive copy function, passing the address of our tail pointer
	new_list->head = copyCharNodesRecursive(source_list->head, &new_tail_ptr);

	//check if head became null during copy (potential malloc failure deep inside)
	if (new_list->head == nullptr && source_list->head != nullptr)
	{
		//copying failed, free the list struct we allocated
		free(new_list);
		return nullptr;
	}

	//assign the tail found during the copy process
	new_list->tail = new_tail_ptr;

	return new_list;
}





void exchangeFirstSecond(MetaList* meta_list)
{
	if (meta_list == nullptr || meta_list->head == nullptr || meta_list->head->next == nullptr) {
		return; //not enough nodes to swap
	}
	MetaListNode* first_node = meta_list->head;
	MetaListNode* second_node = first_node->next;
	MetaListNode* third_node = second_node->next;

	second_node->previous = nullptr;
	second_node->next = first_node;

	first_node->previous = second_node;
	first_node->next = third_node;


	//if there are more than 2 elements, make the 3rd one point to the previous first
	if (third_node != nullptr) {
		third_node->previous = first_node;
	}

	meta_list->head = second_node;

	//if there are only 2 elements, the previous first is now the last
	if (third_node == nullptr) {
		meta_list->tail = first_node;
	}
}


//helper function to find the first significant node (non-zero, non-minus)
//skips leading '-' and '0' characters.
//returns nullptr if the list represents zero (empty, "-", "0", "-0", "00", etc.)
//or if the list is null.
CharListNode* findFirstSignificantNodeRecursive(CharListNode* node) {
	//base case: start of list reached without finding a significant node,
	//or the initial node was null.
	
	//cout << "Node value: " << node->data;
	if (node == nullptr) {
		return nullptr;
	}

	//check the current node's data.
	//'-' sign is handled by intToCharListRecursive adding it at the tail.
	//we only need to skip '0' here when moving backwards from the tail.
	if (node->data == '0') {
		//if it's a character to skip, recurse on the previous node
		//to continue the search backward towards the head.
		return findFirstSignificantNodeRecursive(node->previous);
	}
	else {
		//found the first node (moving backwards) whose data is not '0'.
		//also skip the tail '-' if it's the only thing left.
		if (node->data == '-' && node->previous == nullptr) {
			return nullptr; //treat "-0" or just "-" as zero for comparison magnitude
		}
		//this is the most significant digit's node. Return it.
		return node;
	}
}

//helper function to recursively compare two lists node by node
//nodeA and nodeB point to the first nodes to be compared

int compareListsValueRecursive(CharListNode* nodeA, CharListNode* nodeB, bool areBothNegative) {
	//base case 1: Both lists ended simultaneously (reached null before head). Magnitudes are equal.
	if (nodeA == nullptr && nodeB == nullptr) {
		return 0; //equal magnitude -> equal value since signs are handled outside
	}

	//base case 2: List A ended (shorter magnitude), but List B has more digits.
	if (nodeA == nullptr) {
		//A has smaller magnitude.
		//if negative, A > B. If positive, A < B.
		return areBothNegative ? 1 : -1;
	}

	//base case 3: List B ended (shorter magnitude), but List A has more digits.
	if (nodeB == nullptr) {
		//B has smaller magnitude.
		//f negative, A < B. If positive, A > B.
		return areBothNegative ? -1 : 1;
	}

	//compare Current Digits first
	//at this point, both nodeA and nodeB are non-null.
	
	//cout << "Comparing digits - " << "A: " << nodeA->data << ", B: " << nodeB->data << endl; // Debug print

	if (nodeA->data < nodeB->data) {
		//magnitudes differ at this position.
		//if negative, A > B (e.g., -1 vs -2), if positive, A < B.
		return areBothNegative ? 1 : -1;
	}
	else if (nodeA->data > nodeB->data) {
		//magnitudes differ at this position.
		//if negative, A < B (e.g., -2 vs -1), if positive, A > B.
		return areBothNegative ? -1 : 1;
	}
	else {
		//digits are equal: recurse on the less significant digits
		//the result depends entirely on the rest of the lists.
		
		// cout << "Digits equal, recursing..." << endl;

		return compareListsValueRecursive(nodeA->previous, nodeB->previous, areBothNegative);
	}
}




int charListToIntRecursive(CharListNode* node, int power = 0) {
	//base case: End of the list
	if (node == nullptr) {
		return 0;
	}

	//recursively process the rest of the list first (more significant digits)
	int remaining_val = charListToIntRecursive(node->next, power + 1);

	int digit = node->data - '0';

	//calculate power as 10^position for this digit
	int current_val = digit * (int)pow(10, power);
	return remaining_val + current_val;

	//handle trailing minus sign
	if (node->next == nullptr && node->data == '-') {
		return -remaining_val;
	}

	//ignore non-digit, non-minus characters
	return remaining_val;
}

//0 is the head, then 1, 2, 3 etc
MetaListNode* findMetaNodeAtIndexRecursive(MetaListNode* current_node, int index) {
	//case 1: Node not found
	if (current_node == nullptr) {
		return nullptr;
	}
	//case 2: Found the node
	if (index == 0) {
		return current_node;
	}

	//move to the next node after check
	if (index < 0)
	{
		return nullptr;
	}
	return findMetaNodeAtIndexRecursive(current_node->next, index - 1);
}




void charNodePrintRecursive(CharListNode* node)
{
	if (node == NULL)
	{
		return;
	}
	cout << node->data;
	charNodePrintRecursive(node->next);
}

void charListPrint(CharList* list) {
	if (list) { // Check if the list pointer itself is valid
		charNodePrintRecursive(list->head);
	}
}





void initializeMetaList(MetaList* meta_list) {
	meta_list->head = nullptr;
	meta_list->tail = nullptr;
}

void addFrontMetaList(MetaList* meta_list, CharList* list_value) {
	MetaListNode* node = (MetaListNode*)malloc(sizeof(MetaListNode));

	if (node == nullptr) {
		cout << "malloc failed";
		return;
	}

	node->list_data = list_value;
	node->previous = NULL;
	node->next = meta_list->head;

	if (meta_list->head == NULL) {
		meta_list->head = node;
		meta_list->tail = node;
	}
	else {
		meta_list->head->previous = node;
		meta_list->head = node;
	}
}




void metaNodePrintRecursive(MetaListNode* node, int current_depth) {
	if (node == nullptr) {
		return;
	}

	metaNodePrintRecursive(node->next, current_depth + 1);

	cout << current_depth << ": "; //current_depth is the 0-based index from the top
	charListPrint(node->list_data);
	cout << endl;
}

void metaListPrint(MetaList* meta_list) {
	if (meta_list != nullptr) {
		metaNodePrintRecursive(meta_list->head, 0); //start recursion from head with index 0
	}
}



void addCharToTopList(MetaList* meta_list, char char_to_add) {
	if (meta_list == nullptr || meta_list->head == nullptr) {
		return;
	}
	CharList* top_inner_list = meta_list->head->list_data;
	//check if the list_data itself is null
	if (top_inner_list == nullptr) {
		return;
	}
	addFrontCharList(top_inner_list, char_to_add);
}

void removeFrontCharFromTopList(MetaList* meta_list)
{
	if (meta_list == nullptr || meta_list->head == nullptr) {
		return; //stack null or empty
	}
	CharList* top_inner_list = meta_list->head->list_data;
	if (top_inner_list == nullptr || top_inner_list->head == nullptr) {
		return; //top list null or empty
	}

	CharListNode* node_to_remove = top_inner_list->head;

	top_inner_list->head = node_to_remove->next;

	if (top_inner_list->head == nullptr) {
		//the list is empty
		top_inner_list->tail = nullptr;
	}
	else {
		//the list still has some elements
		top_inner_list->head->previous = nullptr;
	}
	free(node_to_remove);
}


void addTailCharList(CharList* list, char value) {
	CharListNode* node = (CharListNode*)malloc(sizeof(CharListNode));

	node->data = value;
	node->next = nullptr; //tail node's next is always null

	if (list->head == nullptr) {
		//list is empty
		node->previous = nullptr;
		list->head = node;
		list->tail = node;
	}
	else {
		//list isnt empty, add after the current tail
		node->previous = list->tail;
		list->tail->next = node;
		list->tail = node; //update the list's tail pointer
	}
}


void removeTailCharList(CharList* list)
{
	if (list == nullptr || list->tail == nullptr) {
		//list is null or already empty, nothing to remove
		return;
	}
	CharListNode* node_to_remove = list->tail;

	if (list->head == list->tail) {
		//its the only node
		list->head = nullptr;
		list->tail = nullptr;
	}
	else {
		//more than one node exists, update the tail
		list->tail = node_to_remove->previous;
		//detach the new tail from the old one
		list->tail->next = nullptr;
	}
	free(node_to_remove);
}

void intToCharListRecursiveHelper(CharList* list, int value) {
	//no more digits left to process.
	if (value == 0) {
		return;
	}

	//get the current digit character (least significant)
	int current_digit = value % 10;
	char digit_char = (char)('0' + current_digit);

	//recurse on the remaining value first
	int remaining_value = value / 10;
	intToCharListRecursiveHelper(list, remaining_value);

	addFrontCharList(list, digit_char);
}

CharList* intToCharListRecursive(int value) {
	CharList* new_list = (CharList*)malloc(sizeof(CharList));
	if (new_list == nullptr) {
		return nullptr;
	}
	initializeCharList(new_list);

	//handle 0 separately
	if (value == 0) {
		addFrontCharList(new_list, '0');
		return new_list;
	}

	//handle negative values
	if (value < 0) {
		//convert to positive for digit extraction
		int abs_value = -value;
		//build the list of digits
		intToCharListRecursiveHelper(new_list, abs_value);
		//add the minus sign at the tail
		addTailCharList(new_list, '-');
	}
	else {
		//regular positive number
		intToCharListRecursiveHelper(new_list, value);
	}

	if (new_list->head == nullptr && value != 0) {
		free(new_list);
		return nullptr;
	}

	return new_list;
}


//removes the head node (front character) directly from the given CharList.
//this is different from removeFrontCharFromTopList which acts on the MetaList.
void removeHeadCharNode(CharList* list) {
	if (list == nullptr || list->head == nullptr) {
		return; //list null or empty
	}

	CharListNode* node_to_remove = list->head;
	list->head = node_to_remove->next; //update head

	if (list->head == nullptr) {
		//the list became empty
		list->tail = nullptr;
	}
	else {
		//list still has elements, update new head's previous pointer
		list->head->previous = nullptr;
	}
	free(node_to_remove);
}

//recursive Helper to Remove Trailing Zeros
void removeTrailingZerosRecursive(CharList* list) {
	//base case 1: list is invalid, empty, or only has one node left.
	if (list == nullptr || list->head == nullptr || list->tail == nullptr || list->head == list->tail) {
		return;
	}

	//base case 2: the current tail is not '0'. Stop recursion.
	if (list->tail->data != '0') {
		return;
	}

	//the current tail is '0' and it's not the only node. Remove it.

	CharListNode* nodeBeforeTail = list->tail->previous;
	removeTailCharList(list); //frees the old tail node

	//update the list's tail pointer
	list->tail = nodeBeforeTail;

	//ensure the new tail terminates the list correctly
	if (list->tail) {
		list->tail->next = nullptr;
	}
	else {
		list->head = nullptr;
	}

	//make the recursive call to check the new tail
	removeTrailingZerosRecursive(list);
}

//normalization Function (using recursion for tail zero removal)
//removes only trailing zeros (from tail - MSD), preserving leading zeros (at head - LSD).
void normalizeCharList(CharList* list) {
	if (!list || !list->head) {
		return; //nothing to normalize (null or empty list)
	}

	//1. Check and temporarily store sign
	int isNegative = 0;
	if (list->tail && list->tail->data == '-') {
		isNegative = 1;
		if (list->head != list->tail) {
			CharListNode* newTail = list->tail->previous;
			removeTailCharList(list);
			list->tail = newTail;
			if (list->tail) list->tail->next = nullptr;
			else list->head = nullptr;
		}
		else {
			removeTailCharList(list);
			addFrontCharList(list, '0');
			isNegative = 0;
			return;
		}
	}

	//2. Remove all trailing zeros from tail (MSD end)
	removeTrailingZerosRecursive(list);


	//3. Check if the number is effectively zero after MSD zero removal
	int isEffectivelyZero = 0;
	if (list->head == nullptr) {
		isEffectivelyZero = 1;
	}
	else if (list->head == list->tail && list->head->data == '0') {
		isEffectivelyZero = 1;
	}

	//4. Handle the zero case
	if (isEffectivelyZero) {
		if (list->head == nullptr) {
			addFrontCharList(list, '0');
		}
		isNegative = 0;
	}


	//5. Restore sign if needed
	if (isNegative && !isEffectivelyZero) {
		if (list->head && list->tail == nullptr) {
			list->tail = findTailRecursive(list->head);
		}
		addTailCharList(list, '-');
	}

	//7. Final check for tail pointer
	if (list->head && !(list->tail && list->tail->data == '-')) {
		CharListNode* current_tail = findTailRecursive(list->head);
		if (current_tail && current_tail->data != '-') {
			list->tail = current_tail;
		}
		else if (current_tail == nullptr && list->head) { //if the only node was the head node
			list->tail = list->head;
		}
		//ensure non-sign tail terminates
		if (list->tail && list->tail->data != '-' && list->tail->next != nullptr) {
			list->tail->next = nullptr;
		}
	}
	else if (list->head && list->tail && list->tail->data == '-') {
		if (list->tail->previous) list->tail->previous->next = list->tail;
		else list->head = list->tail;
	}
	else if (!list->head) {
		list->tail = nullptr; //list is empty
	}


}



//helper function to recursively check if all remaining nodes in a list segment
//up to a potential '-' sign are '0'.

int checkRemainingZerosRecursive(CharListNode* node) {
	//base case 1: reached the end or the sign marker. All digits checked were zero.
	if (node == nullptr || node->data == '-') {
		return 1;
	}
	//base case 2: Found a non-zero digit.
	if (node->data != '0') {
		return 0;
	}
	//recursive step: current digit is '0', check the rest of the list.
	return checkRemainingZerosRecursive(node->next);
}



//helper: Compares digits starting from LSD (head).
//returns:
//   -1 if |A| < |B|
//    0 if |A| == |B|
//    1 if |A| > |B|

int compareMagnitudeRecursive(CharListNode* nodeA, CharListNode* nodeB) {
	//handle potential signs early
	//temporarily treat the sign node as null for comparison purposes within this call
	CharListNode* effectiveNodeA = nodeA;
	CharListNode* effectiveNodeB = nodeB;
	if (effectiveNodeA && effectiveNodeA->data == '-') effectiveNodeA = nullptr;
	if (effectiveNodeB && effectiveNodeB->data == '-') effectiveNodeB = nullptr;

	//--- base cases ---
	//base case 1: both effective lists ended simultaneously. magnitudes are equal so far.
	if (effectiveNodeA == nullptr && effectiveNodeB == nullptr) {
		return 0;
	}

	//base case 2: a ended (or was just sign), b continues. check if remaining b is just zeros.
	if (effectiveNodeA == nullptr) {
		//use the recursive helper to check the remaining nodes of b
		int b_is_all_zeros = checkRemainingZerosRecursive(effectiveNodeB);
		//if b has non-zero digits remaining, |b| > |a|. otherwise, they are equal so far.
		return b_is_all_zeros ? 0 : -1;
	}

	//base case 3: b ended (or was just sign), a continues. check if remaining a is just zeros.
	if (effectiveNodeB == nullptr) {
		//use the recursive helper to check the remaining nodes of a
		int a_is_all_zeros = checkRemainingZerosRecursive(effectiveNodeA);
		//if a has non-zero digits remaining, |a| > |b|. otherwise, they are equal so far.
		return a_is_all_zeros ? 0 : 1;
	}

	//recursive step
	//both lists have digits at this level. compare the rest of the list (more significant digits) first.
	//pass the original node->next pointers to the recursive call
	int comparisonResult = compareMagnitudeRecursive(nodeA->next, nodeB->next);

	//if the more significant parts already determined the result, return it.
	if (comparisonResult != 0) {
		return comparisonResult;
	}

	//more significant parts were equal, compare the current digits.
	//we know effectiveNodeA and effectiveNodeB are non-null here from base cases.
	int digitA = effectiveNodeA->data - '0';
	int digitB = effectiveNodeB->data - '0';

	if (digitA < digitB) {
		return -1;
	}
	else if (digitA > digitB) {
		return 1;
	}
	else {
		//digits are equal, continue the equality found recursively.
		return 0;
	}
}

//wrapper for magnitude comparison. Ignores signs.
int compareMagnitude(const CharList* listA, const CharList* listB) {
	if (!listA || !listA->head) return (!listB || !listB->head) ? 0 : -1; // handle null/empty A
	if (!listB || !listB->head) return 1; // handle null/empty B (A is non-empty here)

	//start comparison from the head (LSD)
	CharListNode* headA = listA->head;
	CharListNode* headB = listB->head;

	return compareMagnitudeRecursive(headA, headB);
}



//adds two sequences of digits represented by nodes, starting from LSD (head).
//returns the head of the resulting sum list segment (nodes only).
//handles carry.
CharListNode* addCharListsRecursiveHelper(CharListNode* nodeA, CharListNode* nodeB, int carry_in) {
	//ignore '-' sign nodes during addition
	if (nodeA && nodeA->data == '-') nodeA = nullptr;
	if (nodeB && nodeB->data == '-') nodeB = nullptr;

	//base case: both lists processed, and no carry left.
	if (nodeA == nullptr && nodeB == nullptr && carry_in == 0) {
		return nullptr;
	}

	//get current digits (0 if node is null)
	int valA = (nodeA != nullptr) ? (nodeA->data - '0') : 0;
	int valB = (nodeB != nullptr) ? (nodeB->data - '0') : 0;

	//calculate sum and next carry
	int sum = valA + valB + carry_in;
	int digit_result = sum % 10;
	int carry_out = sum / 10;

	//create the node for the current result digit
	CharListNode* newNode = (CharListNode*)malloc(sizeof(CharListNode));
	if (!newNode) { /* Malloc error */ cout << "malloc failed in add helper" << endl; return nullptr; }
	newNode->data = (char)('0' + digit_result);
	newNode->previous = nullptr; //will be linked by the caller level

	//recursively call for the next (more significant) digits
	CharListNode* nextNodeA = (nodeA) ? nodeA->next : nullptr;
	CharListNode* nextNodeB = (nodeB) ? nodeB->next : nullptr;
	newNode->next = addCharListsRecursiveHelper(nextNodeA, nextNodeB, carry_out);

	//link the result from the recursive call back to the current node
	if (newNode->next != nullptr) {
		newNode->next->previous = newNode;
	}

	return newNode; //return the head of the created segment
}

//subtracts B from A (digit sequences), assuming |A| >= |B|.
//returns the head of the resulting difference list segment (nodes only).
//handles borrow.
CharListNode* subtractCharListsRecursiveHelper(CharListNode* nodeA, CharListNode* nodeB, int borrow_in) {
	// ignore '-' sign nodes during subtraction
	if (nodeA && nodeA->data == '-') nodeA = nullptr;
	if (nodeB && nodeB->data == '-') nodeB = nullptr;

	//base case: end of list A (since |A| >= |B|, B must also be ended or irrelevant).
	if (nodeA == nullptr) {
		//if there's still a borrow needed, it implies an error in this: |A| >= |B|.
		if (borrow_in) {
			cout << "Error: Borrow needed at end of subtraction" << endl;
		}
		return nullptr;
	}

	//get current digits
	int valA = (nodeA->data - '0');
	int valB = (nodeB != nullptr) ? (nodeB->data - '0') : 0;

	//apply borrow from previous digit
	valA = valA - borrow_in;

	//check if we need to borrow for the current subtraction
	int borrow_out = 0;
	if (valA < valB) {
		valA = valA + 10; //borrow from next digit
		borrow_out = 1;
	}

	//calculate difference
	int diff = valA - valB;

	//create the node for the current result digit
	CharListNode* newNode = (CharListNode*)malloc(sizeof(CharListNode));
	if (!newNode) 
	{ 
		/* Malloc error */ 
		cout << "malloc failed in subtract helper" << endl; 
		return nullptr; 
	}
	newNode->data = (char)('0' + diff);
	newNode->previous = nullptr; //will be linked by caller

	//recursively call for the next digits
	CharListNode* nextNodeA = (nodeA) ? nodeA->next : nullptr;
	CharListNode* nextNodeB = (nodeB) ? nodeB->next : nullptr;
	newNode->next = subtractCharListsRecursiveHelper(nextNodeA, nextNodeB, borrow_out);

	//link back
	if (newNode->next != nullptr) {
		newNode->next->previous = newNode;
	}

	return newNode;
}



//takes two lists (const pointers), performs addition considering signs.
//returns a new CharList containing the normalized result.
//caller is responsible for freeing the returned list.
CharList* addCharLists(const CharList* listA_const, const CharList* listB_const) {
	//1. Duplicate inputs because normalization modifies lists
	CharList* listA = duplicateCharListRecursive(listA_const);
	CharList* listB = duplicateCharListRecursive(listB_const);
	if (!listA || !listB) 
	{ /* Handle malloc error */ 
		freeCharList(listA); 
		freeCharList(listB); 
		return nullptr; 
	}

	//2. Normalize the duplicates
	normalizeCharList(listA);
	normalizeCharList(listB);

	//3. Determine signs
	int isANegative = (listA->tail != nullptr && listA->tail->data == '-');
	int isBNegative = (listB->tail != nullptr && listB->tail->data == '-');

	CharList* resultList = (CharList*)malloc(sizeof(CharList));
	if (!resultList) 
	
	{ // Handle malloc error 
		freeCharList(listA); 
		freeCharList(listB); 
		return nullptr; 
	}
	initializeCharList(resultList);

	CharListNode* resultHeadNode = nullptr;
	int resultIsNegative = false;

	//4. Perform addition or subtraction based on signs
	if (isANegative == isBNegative) {
		//case 1: Signs are the same (A + B or (-A) + (-B)) -> Add magnitudes
		resultHeadNode = addCharListsRecursiveHelper(listA->head, listB->head, 0);
		resultIsNegative = isANegative; // Result sign is the same as operands
	}
	else {
		//case 2: Signs differ (A + (-B) or (-A) + B) -> Subtract magnitudes
		int magnitudeComparison = compareMagnitude(listA, listB); //compare |A| vs |B|

		if (magnitudeComparison >= 0) { // |A| >= |B|
			resultHeadNode = subtractCharListsRecursiveHelper(listA->head, listB->head, 0);
			resultIsNegative = isANegative; //result sign is A's sign
		}
		else { // |A| < |B|
			resultHeadNode = subtractCharListsRecursiveHelper(listB->head, listA->head, 0);
			resultIsNegative = isBNegative; //result sign is B's sign
		}
	}

	//5. Build the result CharList structure
	resultList->head = resultHeadNode;
	if (resultList->head) {
		//find the actual tail node of the result digits before normalization/sign
		resultList->tail = findTailRecursive(resultList->head);
	}

	//6. Handle case where result is effectively zero (e.g., 5 + (-5))
	if (resultList->head == nullptr) {
		// If arithmetic resulted in null head, it means zero
		addFrontCharList(resultList, '0');
		resultIsNegative = false;
	}

	//7. Normalize the result magnitude (remove leading zeros generated by subtraction)
	normalizeCharList(resultList);

	//8. Add sign if necessary, only if result is not zero
	if (resultIsNegative && !(resultList->head->data == '0' && resultList->head->next == nullptr)) {
		addTailCharList(resultList, '-');
	}
	else {
		//ensure tail is correct even if no sign was added or if normalization changed it
		if (resultList->head) {
			resultList->tail = findTailRecursive(resultList->head);
		}
		else {
			resultList->tail = nullptr; //should already be null if head is null
		}
	}

	//9. Free the duplicates used for calculation
	freeCharList(listA);
	freeCharList(listB);

	//10. Return the newly created result list
	return resultList;
}





void commandHandler(MetaList* meta_list, char command) {
	if (command == '\'')
	{
		//1. Create a new empty CharList
		CharList* new_inner_list = (CharList*)malloc(sizeof(CharList));

		initializeCharList(new_inner_list);

		//2. Push it onto the main stack (MetaList)
		addFrontMetaList(meta_list, new_inner_list);
	}
	else if (command == ',')
	{
		//remove the list from the top of the stack
		if (meta_list == nullptr || meta_list->head == nullptr)
		{
			return;
		}
		removeTopCharList(meta_list);
	}
	else if (command == ':')
	{
		//duplicate the list on top of the stack
		if (meta_list == nullptr || meta_list->head == nullptr)
		{
			return;
		}
		//1. Get the original list from the top
		CharList* original_list = meta_list->head->list_data;

		// 3. Create the duplicate
		CharList* copied_list = duplicateCharListRecursive(original_list);

		//4. Push the pointer to the copy to the main stack
		addFrontMetaList(meta_list, copied_list);
	}
	else if (command == ';')
	{
		//exchange the first 2 lists on the stack
		exchangeFirstSecond(meta_list);
	}
	else if (command == '@')
	{
		// Check if stack has at least one element
		if (meta_list == nullptr || meta_list->head == nullptr) {
			return;
		}

		//get the top list
		CharList* index_list = meta_list->head->list_data;
		if (index_list == nullptr || index_list->head == nullptr) {
			return;
		}

		//convert the ingredients to integer, +1 as this is the index relative to the list one place below the top
		int index = charListToIntRecursive(index_list->head) + 1;

		//find metalistnode at the index
		MetaListNode* target_node = findMetaNodeAtIndexRecursive(meta_list->head, index);
		if (target_node == nullptr || target_node->list_data == nullptr) {
			return;
		}

		//get the list at the index
		CharList* list_to_copy = target_node->list_data;

		//duplicate the list
		CharList* copied_list = duplicateCharListRecursive(list_to_copy);

		//remove the previous list
		removeTopCharList(meta_list);

		//add the new one on top
		addFrontMetaList(meta_list, copied_list);
	}
	else if (command == '&')
	{
		//print everything
		metaListPrint(meta_list);

	}

	//the dot command is handled in the main loop, its easier to do it that way
	//the dot adds a new character to the stack after the command input is finished
	/*else if (command == '.')
	{

	}*/
	else if (command == '>')
	{
		//print the first character on the stack and take off the first list

		if (meta_list == nullptr || meta_list->head == nullptr ||
			meta_list->head->list_data == nullptr || meta_list->head->list_data->head == nullptr)
		{
			//attempt cleanup
			if (meta_list != nullptr && meta_list->head != nullptr) {
				removeTopCharList(meta_list);
			}
			return;
		}

		cout << meta_list->head->list_data->head->data;
		removeTopCharList(meta_list);
	}
	else if (command == '-')
	{
		//adds a minus sign if its missing or removes it if its present

		if (meta_list == nullptr || meta_list->head == nullptr || meta_list->head->list_data == nullptr)
		{
			return;
		}

		CharList* top_inner_list = meta_list->head->list_data;

		if (top_inner_list->tail != nullptr && top_inner_list->tail->data == '-')
		{
			//tail exists and is -, remove it
			removeTailCharList(top_inner_list);
		}
		else
		{
			//tail doesn't exist, or isn't -, add - to the tail
			addTailCharList(top_inner_list, '-');
		}
	}
	else if (command == '^')
	{
		//absolute value - removes the minus sign if its present

		if (meta_list == nullptr || meta_list->head == nullptr || meta_list->head->list_data == nullptr)
		{
			return;
		}

		CharList* top_inner_list = meta_list->head->list_data;

		if (top_inner_list->tail != nullptr && top_inner_list->tail->data == '-')
		{
			removeTailCharList(top_inner_list);
		}
	}
	else if (command == '[')
	{

		//get the top list
		CharList* top_list = meta_list->head->list_data;
		if (top_list->head == nullptr) {
			return;
		}

		//get the first character
		char first_char = top_list->head->data;

		//get the ASCII code
		int first_char_ascii = (int)first_char;

		//remove the top list
		removeTopCharList(meta_list);

		//create a new charlist for the ASCII value
		CharList* new_list = (CharList*)malloc(sizeof(CharList));
		if (!new_list) return;
		initializeCharList(new_list);

		//convert the ASCII value to string representation in the correct order
		//for 'z' (ASCII 122), we want "221" (from right to left)
		if (first_char_ascii == 0) {
			addFrontCharList(new_list, '0');
		}
		else {
			//use the existing recursive helper to build the list of digits
			//this helper adds digits to the front recursively, building the correct order.
			intToCharListRecursiveHelper(new_list, first_char_ascii);
		}

		//push the new list to the stack
		addFrontMetaList(meta_list, new_list);
	}
	else if (command == ']')
	{
		//get the top list
		CharList* top_list = meta_list->head->list_data;
		if (top_list->head == nullptr) {
			return;
		}

		//get the integer representation
		int value = charListToIntRecursive(top_list->head);

		//convert code to character
		char ascii_character = (char)value;

		//remove top list
		removeTopCharList(meta_list);

		//create a new charlist
		CharList* new_inner_list = (CharList*)malloc(sizeof(CharList));
		initializeCharList(new_inner_list);

		//push it onto the main stack (MetaList)
		addFrontMetaList(meta_list, new_inner_list);

		//add character
		addCharToTopList(meta_list, ascii_character);
	}
	else if (command == '$')
	{
		//check for null pointers
		if (meta_list == nullptr || meta_list->head == nullptr || meta_list->head->list_data == nullptr) {
			return;
		}

		//top list
		CharList* top_list = meta_list->head->list_data;
		if (top_list->head == nullptr) {
			return;
		}

		//character from the top list
		char char_to_move = top_list->head->data;

		//remove the top character
		removeFrontCharFromTopList(meta_list);

		//create a new charlist
		CharList* new_inner_list = (CharList*)malloc(sizeof(CharList));
		initializeCharList(new_inner_list);

		//push it onto the main stack (MetaList)
		addFrontMetaList(meta_list, new_inner_list);

		//add character
		addCharToTopList(meta_list, char_to_move);
	}
	else if (command == '#')
	{
		//check for at least 2 lists
		if (meta_list == nullptr || meta_list->head == nullptr || meta_list->head->next == nullptr) {
			return;
		}

		MetaListNode* node_A = meta_list->head; //this one will be removed
		MetaListNode* node_B = meta_list->head->next; //we will add node_A to this one

		//check if both lists exist and have valid data
		if (node_A->list_data == nullptr || node_B->list_data == nullptr) {
			return;
		}

		//get the values
		CharList* list_A = node_A->list_data;
		CharList* list_B = node_B->list_data;

		//check if lists have nodes
		if (list_A->head == nullptr) {
			//if A is empty, just remove it
			meta_list->head = node_B;
			node_B->previous = nullptr;
			free(list_A);
			free(node_A);
			return;
		}

		//if B is empty, just move A's contents to B
		if (list_B->head == nullptr) {
			list_B->head = list_A->head;
			list_B->tail = list_A->tail;

			//remove node_A
			meta_list->head = node_B;
			node_B->previous = nullptr;

			//free list_A structure but not its nodes
			node_A->list_data = nullptr;
			free(list_A);
			free(node_A);
			return;
		}

		//link B's tail to A's head
		list_B->tail->next = list_A->head;

		//link A's head back to B's tail
		list_A->head->previous = list_B->tail;

		//update B's tail to be A's original tail
		list_B->tail = list_A->tail;

		//remove node_A and list_A's struct but not list_A's nodes
		meta_list->head = node_B;
		node_B->previous = nullptr;

		//list_A's nodes are now part of list_B, so we can free
		node_A->list_data = nullptr;
		free(list_A);
		free(node_A);
	}
	else if (command == '<')
	{
		//pop A, pop B. If B < A, push 1, else push 0.
		if (meta_list == nullptr || meta_list->head == nullptr || meta_list->head->next == nullptr) { /* Error check */ return; }
		MetaListNode* meta_node_A = meta_list->head;
		MetaListNode* meta_node_B = meta_list->head->next;
		if (meta_node_A->list_data == nullptr || meta_node_B->list_data == nullptr) { /* Error check */ return; }
		CharList* list_A = meta_node_A->list_data;
		CharList* list_B = meta_node_B->list_data;

		//determine signs (from tail)
		int isANegative = (list_A->tail != nullptr && list_A->tail->data == '-');
		int isBNegative = (list_B->tail != nullptr && list_B->tail->data == '-');

		//find significant nodes to check for zero magnitude
		CharListNode* startNodeA = isANegative ? list_A->tail->previous : list_A->tail;
		CharListNode* startNodeB = isBNegative ? list_B->tail->previous : list_B->tail;
		CharListNode* significant_node_A = findFirstSignificantNodeRecursive(startNodeA);
		CharListNode* significant_node_B = findFirstSignificantNodeRecursive(startNodeB);
		int isAMagnitudeZero = (significant_node_A == nullptr);
		int isBMagnitudeZero = (significant_node_B == nullptr);

		int isBLessThanA; //result flag

		//case 1: Check if B represents zero
		if (isBMagnitudeZero) {
			//B is 0 or -0.
			//B < A is only true if A is positive and non-zero.
			isBLessThanA = (!isAMagnitudeZero && !isANegative);
		}
		//case 2: Check if A represents zero (and B is non-zero from above)
		else if (isAMagnitudeZero) {
			//A is 0 or -0, B is non-zero.
			//B < A is true only if B is negative.
			isBLessThanA = isBNegative;
		}
		//case 3: Neither A nor B represents zero. Now compare signs and values.
		else if (isBNegative && !isANegative) {
			//B negative, A positive (both non-zero). B < A is true.
			isBLessThanA = 1;
		}
		else if (!isBNegative && isANegative) {
			//B positive, A negative (both non-zero). B < A is false.
			isBLessThanA = 0;
		}
		else {
			//signs are the same, both non-zero. Use comparison function
			//compare values: returns -1 if A<B, 0 if A==B, 1 if A>B numerically
			int valueComparison = compareListsValueRecursive(significant_node_A, significant_node_B, isANegative); //isANegative represents sign for both

			//we need B < A, which is equivalent to A > B.
			//comparison function returns 1 if A > B.
			isBLessThanA = (valueComparison == 1);
		}


		removeTopCharList(meta_list); //removes A
		removeTopCharList(meta_list); //removes B
		int resultValue = isBLessThanA ? 1 : 0;

		//create and push the result list with '0' or '1'
		char resultChar = (resultValue == 1) ? '1' : '0';
		CharList* result_list = (CharList*)malloc(sizeof(CharList));
		if (result_list == nullptr) 
		{
			//handle error
			return; 
		}
		initializeCharList(result_list);
		addFrontCharList(result_list, resultChar);
		addFrontMetaList(meta_list, result_list);
	}
	else if (command == '=')
	{
		//1. Check for at least two lists (A is top, B is second)
		if (meta_list == nullptr || meta_list->head == nullptr || meta_list->head->next == nullptr) {
			return; //error: Not enough lists
		}

		//2. Get meta nodes and check list_data validity
		MetaListNode* meta_node_A = meta_list->head;
		MetaListNode* meta_node_B = meta_list->head->next;

		if (meta_node_A->list_data == nullptr || meta_node_B->list_data == nullptr) {
			//decide cleanup if needed, for now just return
			return; //error: list data missing
		}

		CharList* list_A = meta_node_A->list_data;
		CharList* list_B = meta_node_B->list_data;

		//3. Determine potential signs (check the TAIL node)
		int isANegative = (list_A->tail != nullptr && list_A->tail->data == '-');
		int isBNegative = (list_B->tail != nullptr && list_B->tail->data == '-');

		//4. Determine if magnitudes are zero by finding the most significant digit
		//find the starting node for magnitude check (skip tail '-' if present)
		CharListNode* startNodeA = isANegative ? list_A->tail->previous : list_A->tail;
		CharListNode* startNodeB = isBNegative ? list_B->tail->previous : list_B->tail;

		//find the actual most significant digit node (will be null if magnitude is zero)
		CharListNode* significant_node_A = findFirstSignificantNodeRecursive(startNodeA);
		CharListNode* significant_node_B = findFirstSignificantNodeRecursive(startNodeB);

		int isAMagnitudeZero = (significant_node_A == nullptr);
		int isBMagnitudeZero = (significant_node_B == nullptr);

		int areEqual; //result flag

		//5. Perform comparison logic
		if (isAMagnitudeZero && isBMagnitudeZero) {
			// Case 1: Both magnitudes are zero. They are equal regardless of sign.
			areEqual = 1;
		}
		else if (isAMagnitudeZero || isBMagnitudeZero) {
			//case 2: One magnitude is zero, the other is not. They are not equal.
			areEqual = 0;
		}
		else {
			//case 3: Neither magnitude is zero. Now signs *and* magnitude matter.
			if (isANegative != isBNegative) {
				//signs differ, and magnitudes are non-zero. Not equal.
				areEqual = 0;
			}
			else {
				//signs are the same, magnitudes non-zero. Compare magnitudes precisely.
				int magnitudeComparison = compareListsValueRecursive(significant_node_A, significant_node_B, isANegative);
				//equal only if the magnitude comparison result is 0
				areEqual = (magnitudeComparison == 0);
			}
		}

		//6. Remove the original top two lists (A then B)
		removeTopCharList(meta_list); //removes A
		removeTopCharList(meta_list); //removes B

		//7. Determine the integer result (1 if equal, 0 if not)
		int resultValue = areEqual ? 1 : 0;

		//8. Create a new CharList representing the integer result (0 or 1)
		CharList* result_list = intToCharListRecursive(resultValue);

		if (result_list == nullptr) {
			cout << "Error creating result list for '=' command." << endl;
			result_list = (CharList*)malloc(sizeof(CharList));
			if (result_list) {
				initializeCharList(result_list);
				addFrontCharList(result_list, '0');
			}
		}

		//9. Push the new result list onto the stack
		addFrontMetaList(meta_list, result_list);
	}
	else if (command == '!')
	{
		//get the top list
		MetaListNode* top_node = meta_list->head;
		CharList* original_list = top_node->list_data;

		//condition: first list empty or containing '0'
		char condition = '0';

		if (original_list->head == nullptr)
		{
			condition = '1';
		}
		//check if '0' is the only character
		else if (original_list->head->data == '0' && original_list->head->next == nullptr)
		{
			condition = '1';
		}

		//remove the top list
		removeTopCharList(meta_list);

		//create a new charlist
		CharList* new_inner_list = (CharList*)malloc(sizeof(CharList));
		initializeCharList(new_inner_list);

		//push it onto the main stack (MetaList)
		addFrontMetaList(meta_list, new_inner_list);

		//add character
		addCharToTopList(meta_list, condition);
	}
	// ~ command is covered in processsingleinstruction
	//else if (command == '~')
	//{
	//	//convert the instruction index into a charlist
	//	CharList* index_list = intToCharListRecursive(instruction_index);

	//	//push it onto the stack
	//	addFrontMetaList(meta_list, index_list);
	//}
	//the same for ? command
	else if (command == '+')
	{
		//add: Pop B, Pop A, Push (A + B)
		//1. Check for at least two lists
		if (meta_list == nullptr || meta_list->head == nullptr || meta_list->head->next == nullptr) {
			//error: Not enough lists
			return;
		}

		//2. Get the top two lists (A is top, B is second)
		MetaListNode* meta_node_A = meta_list->head;
		MetaListNode* meta_node_B = meta_list->head->next;

		CharList* list_A = meta_node_A->list_data;
		CharList* list_B = meta_node_B->list_data;

		//3. Perform the addition (this handles normalization and signs internally)
		CharList* sum_list = addCharLists(list_B, list_A); //B is first arg, A is second -> B + A

		//4. Remove the original two lists (A then B)
		removeTopCharList(meta_list); // Removes A
		removeTopCharList(meta_list); // Removes B

		//5. Push the new sum list onto the stack
		addFrontMetaList(meta_list, sum_list);
	}


}


int processSingleInstruction(MetaList* meta_list, char instructions_string[], int current_index, int length) {

	char current_char = instructions_string[current_index];

	//execute one by one
	int next_instruction_index = current_index + 1;

	//special handling for the ? command
	if (current_char == '?') {

		MetaListNode* node_T = meta_list->head; //top element
		MetaListNode* node_W = meta_list->head->next; //second element

		//get the charlists
		CharList* list_T = node_T->list_data;
		CharList* list_W = node_W->list_data;

		//convert T to int
		int target_index = charListToIntRecursive(list_T->head);

		//check condition: W not empty and not '0'
		int condition = 0;

		if (list_W->head != nullptr)
		{
			if (!(list_W->head->data == '0' && list_W->head->next == nullptr))
			{
				condition = 1;
			}
		}

		//remove T and W
		removeTopCharList(meta_list);
		removeTopCharList(meta_list);

		//determine the index
		if (condition) {
			next_instruction_index = target_index; //perform the jump
		}
		//else: normal one by one execution remains

		//return the calculated next index: target_index or current_index + 1
		return next_instruction_index;

	}
	//handle ~ command
	else if (current_char == '~')
	{
		//convert to charlist
		CharList* index_list = intToCharListRecursive(current_index);

		//push to the top
		if (index_list != nullptr)
		{
			addFrontMetaList(meta_list, index_list);
		}

		return next_instruction_index;
	}
	//handle . command
	else if (current_char == '.')
	{
		char char_to_add;
		cin.get(char_to_add); // Read only ONE character from input

		// Add the character if the read was successful and not newline
		if (cin.good() && char_to_add != '\n') {
			addCharToTopList(meta_list, char_to_add);
		}

		//consume newline if present after the character - fixes the issue on my computer that sometimes it registers the wrong input
		if (char_to_add != '\n' && cin.peek() == '\n') {
			cin.ignore();
		}
		return next_instruction_index;
	}

	//define command characters
	else if (current_char == '\'' || current_char == ',' || current_char == ':' || current_char == ';' || current_char == '@' ||
		current_char == '&' || current_char == '>' || current_char == '-' || current_char == '^' || current_char == '[' ||
		current_char == ']' || current_char == '$' || current_char == '#' || current_char == '<' || current_char == '=' ||
		current_char == '!' || current_char == '+') {
		commandHandler(meta_list, current_char);
		return next_instruction_index;
	}
	else {
		//regular character: Add to the list at the top of the main stack
		addCharToTopList(meta_list, current_char);
		return next_instruction_index;
	}
}


void executeInstructionsRecursive(MetaList* meta_list, char instructions_string[], int current_index, int length) {
	//stop recursion
	if (current_index < 0 || current_index >= length) {
		return;
	}

	//process the instruction at the current index and get the index for the next step
	int next_index = processSingleInstruction(meta_list, instructions_string, current_index, length);

	//recursive call to execute the next instruction
	executeInstructionsRecursive(meta_list, instructions_string, next_index, length);
}


int main()
{
	MetaList main_stack;
	initializeMetaList(&main_stack); //initialize the main stack

	char instructions_string[MAX_INSTRUCTION_LENGTH]; //buffer to hold the input line
	int length = 0; //current number of characters in the buffer

	//this loop reads the input line into the buffer.
	//its restricted to reading only, processing happens later.
	for (int i = 0; i < MAX_INSTRUCTION_LENGTH; ++i) // Use ++i for convention
	{
		char current_char;
		cin.get(current_char); //read one character

		//check if the newline character was read
		if (current_char == '\n')
		{
			//newline signifies end of the command line.
			//process the entire buffer if it contains anything.
			if (length > 0)
			{
				executeInstructionsRecursive(&main_stack, instructions_string, 0, length);
			}
			//length = 0;
			break;
		}
		else //any other character (including '.' and other commands)
		{
			//store the character in the buffer
			instructions_string[length] = current_char;
			length++;
		}
	}
}