//
// Created by Harshit Gupta and Manya Verma on 15.01.2023.
//

#include "Akinator.h"


bool Akinator::contains(T item) {
    assert(item.string != nullptr);

    Node* result = search_(item, root);

    return result != nullptr;
}


Stack Akinator::elements() {
    elements_(root);
    Stack result = Elements;
    Elements.clear();

    return result;
}


bool Akinator::erase(T item){
    if (this->contains(item)) {
        root = erase_(item, root);
        size--;

        return true;
    }

    return false;
}


void Akinator::destroy_tree_(Node* node) {
    if(node != nullptr){
        destroy_tree_(node->left);
        destroy_tree_(node->right);
        delete node;
    }
}


void Akinator::f_round_(Node* node){
    if(node != nullptr) {
        if (node->question) node->data.string[0] = '?';

        size_t length = strlen(node->data.string);

        node->data.string[length - 1] = '.';

        strcat (f_round_str, node->data.string);
        strcat (f_round_str, "\n");

        f_round_(node->left);
        f_round_(node->right);
    }
}


int Akinator::get_height_(const Node *node) const{
    if (node == nullptr) return 0;

    int lHeight = get_height_(node->left);
    int rHeight = get_height_(node->right);

    if(lHeight > rHeight) return (lHeight + 1);
    else return (rHeight + 1);
}


void Akinator::elements_(Node* node){
    if(node != nullptr){
        elements_(node->left);
        Elements.push(node->data);
        elements_(node->right);
    }
}


int Akinator::do_question(Node* node) {
    assert(node != nullptr);

    char answer = 0;
    if (node->question) {
        sprintf(freebuf, "Is the country %s?\n Press y (yes) or n (no)\n", node->data.string);
        speak(freebuf);
        if (!scanf("\n%c", &answer)) {
            printf("%c\n", answer);
            return INVALID_ANSWER;
        }

        while (answer != 'y' && answer != 'n' && answer != 'e') {
            RED_TEXT_COLOR

            speak("Invalid answer! Please try again or press e to return to the main menu\n");
            scanf("%c\n", &answer);

            DEFAULT_TEXT_COLOR
        }
        if      (answer == 'y') return do_question(node->left);
        else if (answer == 'n') return do_question(node->right);
        else if (answer == 'e') return ALL_OK;
    }

    sprintf(freebuf, "Is it %s?\n Press y (yes) or n (no)\n", node->data.string);
    speak(freebuf);
    if (!scanf("\n%c", &answer)) return INVALID_ANSWER;

    while (answer != 'y' && answer != 'n' && answer != 'e') {
        RED_TEXT_COLOR

        speak("Invalid answer! Please try again or press e to exit\n");
        scanf("%c\n", &answer);

        DEFAULT_TEXT_COLOR
    }

    if      (answer == 'y') {
        CYAN_TEXT_COLOR
        speak("Haha!!! I won the game! It was so fucking easy!\n");
        DEFAULT_TEXT_COLOR
    }
    else if (answer == 'n') return Add_new_object(node);
    else if (answer == 'e') return ALL_OK;


    return ALL_OK;
}


int Akinator::Add_new_object (Node* node) {
    assert(node != nullptr);

    speak("I'm so sorry, I don't know what did you conceive.\n Let me know, what object are you talking about?\n");

    size_t _size = LIMITED_SIZE_OF_STRING * 2;
    char* new_object = new char[LIMITED_SIZE_OF_STRING];
    getline(&freebuf, &_size, stdin);
    if (!getline(&new_object, &_size, stdin)) return INVALID_ANSWER;

    if (this->contains(simple_string(new_object))) {
        RED_TEXT_COLOR

        speak("Sorry, but the element is in the database already, so you did the mistake.\n"
              "Please, try to find your country again.");

        DEFAULT_TEXT_COLOR

        return ALL_OK;
    }

    while (new_object[0] < 'A' || new_object[0] > 'Z') {
        RED_TEXT_COLOR

        speak("Invalid input! Please try again!\n"
              "Check that name of the country have to start with huge letter");
        getline(&new_object, &_size, stdin);

        DEFAULT_TEXT_COLOR
    }

    node->left = new Node;
    node->left->data.string = new_object;

    size_t length = strlen(new_object);
    node->left->data.string[length - 1] = '\n';

    node->right = new Node;
    node->right->data = node->data;

    node->data.string = new char[LIMITED_SIZE_OF_STRING * 2];
    char* temp_str = node->data.string;
    temp_str++;

    speak("Which fact does distinguish your object?\n");
    if (!getline(&temp_str, &_size, stdin)) return INVALID_ANSWER;
    length = strlen(node->data.string);
    node->data.string[length - 1] = '\n';
    node->data.string[0] = '\t';
    node->question = true;

    GREEN_TEXT_COLOR

    speak("Thank you for game! See you later!\n");
    size += 2;

    DEFAULT_TEXT_COLOR

    if (verification()) return verification();

    return ALL_OK;
}


Akinator::Akinator() {
    root = new Node;
    root->data.string = new char[LIMITED_SIZE_OF_STRING];
    root->data.string = (char*) "Nobody\n";
    root->data.size = strlen("Nobody\n");
    f_round_str = nullptr;
}


Akinator::Akinator(const char* input) {
    root = new Node;
    FILE* database = fopen(input, "rb");
    size_t size_of_file = Size_of_file(database);
    size_t number_of_lines = 0;
    text = text_from_file(database, size_of_file, &number_of_lines);

    size = (int) number_of_lines;
    Read_in_tree(&number_of_lines, root);

    fclose(database);
}

Node* Akinator::erase_(T item, Node* node) {
    if (node == nullptr) return node;

    return nullptr;
}

void Akinator::destroy_tree() {
    f_round_str = (char*) calloc(size + 5, 2 * LIMITED_SIZE_OF_STRING);
    assert(f_round_str != nullptr);

    size_t lenstr = (size + 5) * 2 * LIMITED_SIZE_OF_STRING;

    f_round_(root);
    f_round_str[lenstr - 1] = '\0';

    FILE* output = fopen("database.txt", "wb");
    fputs(f_round_str, output);
    fclose(output);
    destroy_tree_(root);
}


void Akinator::Read_in_tree(size_t* number_of_lines, Node* node) {
    assert(text != nullptr);

    if (*number_of_lines <= 0) return;
    (*number_of_lines)--;

    if (text[0] == '?') {
        node->question = true;
        text[0] = '\t';
    }

    node->data.string = text;
    if (!node->question) return;

    node->left = new Node;
    text = strchar(text, '\0');
    text++;

    Read_in_tree(number_of_lines, node->left);

    node->right = new Node;
    text = strchar(text, '\0');
    text++;

    Read_in_tree(number_of_lines, node->right);

}

char* strchar(char* buffer, char sym) {
    char* c = buffer;
    while (*c != sym) {
        c++;
        if (c == nullptr || *c == '\0') return c;
    }

    return c;
}

void Akinator::_dumpE (Node* node) const {
    if (node == nullptr) return;

    char* tmp_string = (char*) calloc(15, LIMITED_SIZE_OF_STRING);

    char* color = nullptr;

    if (node->question) color = (char*) GOLD;
    else                color = (char*) LIME;

    sprintf(tmp_string,
            "node%p[ style = \"filled, rounded\", fillcolor = \"%s\"]"
            "node%p [label=\"<f0> name (%s)|<f1> address (%p)|<f2> left (%p)|<f3> right (%p)\"];\n",
            node, color, node, node->data.string, node, node->left, node->right);

    strcat(f_round_str, tmp_string);

    if (node->left  != nullptr) _dumpE(node->left);
    if (node->right != nullptr) _dumpE(node->right);

    free(tmp_string);

}

void Akinator::_dumpV (Node* node) const {
    if (node != nullptr) {
        char* temp_string = (char*) calloc(15, LIMITED_SIZE_OF_STRING);
        if (node->right != nullptr) {
            sprintf(temp_string, "node%p:f3 -> node%p:f0;\n", node, node->right);
            strcat(f_round_str, temp_string);
            _dumpV(node->right);
        }

        if (node->left  != nullptr) {
            sprintf(temp_string, "node%p:f2 -> node%p:f0;\n", node, node->left);
            strcat(f_round_str, temp_string);
            _dumpV(node->left);
        }

        free(temp_string);
    }

}

int Akinator::Dump() {
    if (verification()) return verification();

    FILE* Graph = fopen("Graph.dot", "wb");
    if(Graph == nullptr) fprintf(stderr, "Error opening file!\n");

    f_round_str = (char*) calloc(size, LIMITED_SIZE_OF_STRING * LIMITED_SIZE_OF_STRING);
    if (f_round_str == nullptr) return INVALID_MEM_ALLOC;


    const char* DEFAULT_TEXT = "digraph G{\n"
                               "edge [color = \"darkgreen\"];\n"
                               "rankdir = \"LR\";\n"
                               "node [shape = record];\n"
                               "node [color = \"#000000\", shape = record];\n";
    strcat(f_round_str, DEFAULT_TEXT);

    _dumpE(root);
    _dumpV(root);

    fputs(f_round_str, Graph);
    fputs("}\n", Graph);

    fclose(Graph);

    char*  command = (char*) calloc(2, LIMITED_SIZE_OF_STRING);
    if (command == nullptr) return INVALID_MEM_ALLOC;

    strcpy (command, DOT);
    strcat (command, OUTPUT_FILE_NAME);

    system (command);

    char* command2 = (char*) calloc(2, LIMITED_SIZE_OF_STRING);
    if (command2 == nullptr) return INVALID_MEM_ALLOC;

    strcat(command2, "eog ");
    strcat(command2, OUTPUT_FILE_NAME);
    system (command2);

    free(command);

    if (verification()) return verification();

    return ALL_OK;
}

int Akinator::check (Node* node) const{
    if (node == nullptr) return ALL_OK;

    if (node->question) {
        if (node->left == nullptr || node->right == nullptr) {
            fprintf(stderr, "Invalid pointer...\n");
            return INVALID_POINTER;
        }
    }
    else {
        if (node->left != nullptr || node->right != nullptr) {
            fprintf(stderr, "Invalid pointer...\n");
            return INVALID_POINTER;
        }
    }

    int error_code = ALL_OK;
    error_code = check(node->left);
    if (error_code) return error_code;

    error_code = check(node->right);
    if (error_code) return error_code;

    return ALL_OK;
}

Stack Akinator::definition(const char* object) {
    Elements.clear();
    if (object == nullptr) {
        fprintf(stderr, "Invalid object in input!\n");
        return Elements;
    }

    simple_string ss;
    ss.string = (char*) object;

    char* speech = (char*) calloc(size + 1, LIMITED_SIZE_OF_STRING + RESERVE_MEM);
    char* word   = (char*) calloc(2, LIMITED_SIZE_OF_STRING);

    if (speech == nullptr || word == nullptr) {
        fprintf(stderr, "Memory allocation error!\n");
        return Elements;
    }

    strcat(speech, "espeak ");

    Node* result = search(ss);

    if (result != nullptr) {
        char name[30];
        name[0] = '\0';
        strcat(name, "espeak ");
        strcat(name, object);
        system(name);

        strcpy(word, Elements.at(0).string);
        printf("%s is\n1)", object);
        strcat(speech, word);

        size_t number_of_words = Elements.Get_Size();
        int shift = 0;
        for (int i = 1; i < number_of_words; ++i) {
            if (strncmp("\tnot\tlocated", Elements.at(i).string, 11)) {
                sprintf(word, "%d)%s", i + 1 - shift, Elements.at(i).string);
                strcat(speech, word);
            }
            else shift++;
        }

        printf("%s", speech + ESPEAK_SIZE - 1);
        printf("\n");
    }
    else {
        speak("Object wasn't found!");
        return elements();
    }

    for (int i = ESPEAK_SIZE; i < strlen(speech); ++i) {
        if (!std::isalpha(speech[i])) speech[i] = '_';
    }

    strcat(speech, " -s 110");

    system(speech);

    free(word);
    free(speech);
    return Elements;
};


int speak(const char* string) {
    assert(string);

    puts(string);

    char* speech = (char*) calloc(10 + strlen(string), sizeof(char));
    if (speech == nullptr) return INVALID_MEM_ALLOC;

    strcpy(speech, "espeak ");
    strcat(speech, string);
    replace_spaces(speech);

    system(speech);

    free(speech);

    return ALL_OK;
}

int Akinator::verification() const {
    return check(root);
}

int Akinator::Show_database() const {
    if (verification()) return verification();

    FILE* Graph = fopen("UserGraph.dot", "wb");
    if(Graph == nullptr) fprintf(stderr, "Error opening file!\n");

    char* uf_round_str = (char*) calloc(size, LIMITED_SIZE_OF_STRING * LIMITED_SIZE_OF_STRING);
    if (uf_round_str == nullptr) return INVALID_MEM_ALLOC;


    const char* DEFAULT_TEXT = "digraph G{\n"
                               "edge [color = \"darkgreen\"];\n"
                               "rankdir = \"LR\";\n"
                               "node [shape = Mcircle];\n"
                               "node [color = \"#000000\", shape = Mcircle];\n";
    strcat(uf_round_str, DEFAULT_TEXT);

    _udumpE(root, uf_round_str);
    _udumpV(root, uf_round_str);

    fputs(uf_round_str, Graph);
    fputs("}\n", Graph);

    fclose(Graph);

    char*  command = (char*) calloc(2, LIMITED_SIZE_OF_STRING);
    if (command == nullptr) return INVALID_MEM_ALLOC;

    strcpy (command, UDOT);
    strcat (command, DATABASE_TREE);

    system (command);

    char* command2 = (char*) calloc(2, LIMITED_SIZE_OF_STRING);
    if (command2 == nullptr) return INVALID_MEM_ALLOC;

    strcat(command2, "eog ");
    strcat(command2, DATABASE_TREE);
    system (command2);

    free(command);

    free(uf_round_str);
    if (verification()) return verification();

    return ALL_OK;
}

void Akinator::_udumpE (Node* node, char* uf_round_str) const {
    if (node == nullptr) return;

    char* tmp_string = (char*) calloc(LIMITED_SIZE_OF_STRING, LIMITED_SIZE_OF_STRING);

    char* color = nullptr;

    if (node->question) color = (char*) GOLD;
    else                color = (char*) LIME;

    sprintf(tmp_string,
            "node%p[ style = \"filled, rounded\", fillcolor = \"%s\"]"
            "node%p [label=\"%s\"];\n",
            node, color, node, node->data.string);

    strcat(uf_round_str, tmp_string);

    if (node->left  != nullptr) _udumpE(node->left,  uf_round_str);
    if (node->right != nullptr) _udumpE(node->right, uf_round_str);

    free(tmp_string);
}

void Akinator::_udumpV (Node* node, char* uf_round_str) const {
    if (node != nullptr) {
        char* temp_string = (char*) calloc(LIMITED_SIZE_OF_STRING, LIMITED_SIZE_OF_STRING);
        if (node->right != nullptr) {
            sprintf(temp_string, "node%p -> node%p [label=\"false\"];\n", node, node->right);
            strcat(uf_round_str, temp_string);
            _udumpV(node->right, uf_round_str);
        }

        if (node->left  != nullptr) {
            sprintf(temp_string, "node%p -> node%p [label=\"true\"];\n", node, node->left);
            strcat(uf_round_str, temp_string);
            _udumpV(node->left, uf_round_str);
        }

        free(temp_string);
    }
}


int Akinator::Compare(const char* lhs, const char* rhs) {
    Stack def_lhs(16);
    Stack def_rhs(16);

    def_lhs.copy(definition(lhs));
    def_rhs.copy(definition(rhs));

    if (def_lhs.at(0).string == nullptr || def_rhs.at(0).string == nullptr) return INVALID_OBJECT;

    Stack result(16);
    simple_string str;

    int i = 0;
    while (!str_cmp(def_rhs.at(i).string, def_lhs.at(i).string)) result.push(def_rhs.at(i++));

    char* speech = (char*) calloc(size + 1, LIMITED_SIZE_OF_STRING + RESERVE_MEM);
    char* word   = (char*) calloc(2, LIMITED_SIZE_OF_STRING);

    size_t number_of_words = result.Get_Size();
    for (int j = 0; j < number_of_words; ++j) {
        sprintf(word, "%d)%s", j + 1, Elements.at(j).string);
        strcat(speech, word);
    }
    if (number_of_words == 0) strcat(speech, "None\n");

    speak("Similarities:\n");

    GREEN_TEXT_COLOR

    speak(speech);

    DEFAULT_TEXT_COLOR

    speak("Differences:\n");

    YELLOW_TEXT_COLOR
    sprintf(speech, "%s is %s\n", lhs, def_lhs.at(i).string);
    speak(speech);

    CYAN_TEXT_COLOR

    sprintf(speech, "%s is %s\n", rhs, def_rhs.at(i).string);
    speak(speech);

    DEFAULT_TEXT_COLOR

    free(speech);
    free(word);

    return ALL_OK;
}


