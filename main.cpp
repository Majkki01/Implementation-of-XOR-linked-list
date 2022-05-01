#include <iostream>
#include <string>

using namespace std;


typedef struct list {
    int value;
    list *Xor;
};

typedef struct moving{
    list* act = NULL;
    list* prev = NULL;
    list* next = NULL;
};

list* XOR(struct list* a, struct list* b) {
    return (list*)((int)(a) ^ (int)(b));
}


void add_(list** head, int value, list** tail, moving* move, bool begin) {
    list* x = new list;

    x->Xor = *head;
    x->value = value;

    if (*head != NULL) {
        (*head)->Xor = XOR(x,(*head)->Xor);
    }

    if (*tail == NULL) {
        *tail = x;
    }
    
    if (move->act == NULL)  move->act = x;
    else if (move->prev == NULL && begin) move->prev = x;
    else if (move->next == NULL && !begin) move->next = x;
    
    *head = x;
}


void print(list* head) {
    if (head != NULL) {
        list* curr = head;
        list* prev = NULL;
        list* next;

        while (curr != NULL) {
            cout << curr->value << " ";
            next = XOR(prev, curr->Xor);
            prev = curr;
            curr = next;
        }
        cout << endl;
    }
    else cout << "NULL" << endl;
}


void next_(moving* move, list* head) {
    if (move->act != NULL) {
        if (move->next != NULL) {
            move->prev = move->act;
            move->act = move->next;
            move->next = XOR(move->prev, move->act->Xor);
        }
        else {
            move->prev = NULL;
            move->act = head;
            move->next = head->Xor;
        }
    }
}


void prev_(moving* move, list* tail) {
    if (move->act != NULL) {
        if (move->prev != NULL) {
            move->next = move->act;
            move->act = move->prev;
            move->prev = XOR(move->next, move->act->Xor);
        }
        else {
            move->next = NULL;
            move->act = tail;
            move->prev = tail->Xor;
        }
    }
}


void del_(list** head, list** tail, moving* move, bool begin) {
    if (*head != NULL) {
        if ((*head)->Xor != NULL) {
            if (*head == move->act && begin) prev_(move, *tail);
            else if (*head == move->act && !begin) prev_(move, *head);
            if (*head == move->prev && begin) move->prev = NULL;
            else if (*head == move->next && !begin) move->next = NULL;

            list* tmp = XOR(*head, (*head)->Xor->Xor);
            void* x = *head;
            *head = (*head)->Xor;
            (*head)->Xor = tmp;
            delete x;
        }
        else {
            move->act = NULL;
            move->next = NULL;
            move->prev = NULL;

            (*head)->value = NULL;
            (*head)->Xor = NULL;
            *head = NULL;

            (*tail)->value = NULL;
            (*tail)->Xor = NULL;
            *tail = NULL;
        }
    }
}


void _act(list** head, list** tail, moving* move, int value, bool add) {
    if ((*head == NULL || *head == move->act) && add == 1) {
        add_(head, value, tail, move, 1);
    }
    else if (*head != NULL) {
        if (add == 0 && *head == move->act) del_(head, tail, move, 1);
        else if (add == 0 && *tail == move->act) del_(tail, head, move, 0);
        else {
            list* tmp_head = *head, *prev = *head;
            *head = (*head)->Xor;
            list *next = XOR(prev, (*head)->Xor);

            while (*head != move->act) {
                next = XOR(prev, (*head)->Xor);
                prev = *head;
                *head = next;
            }

            (*head)->Xor = XOR((*head)->Xor, prev);

            if (add == 1) {
                add_(head, value, tail, move, 1);
                move->prev = *head;
            }
            else {
                del_(head, tail, move, 1);
                move->act = prev;
                move->next = *head;
            }

            list* tmp_new = *head;
            (*head)->Xor = XOR(prev, (*head)->Xor);
            *head = prev;
            if (*head == tmp_head) tmp_head->Xor = (*head)->Xor = tmp_new;
            else (*head)->Xor = (XOR(XOR((*head)->Xor, next), tmp_new));

            *head = tmp_head;
        }
    }
}


void del_val(list** head, list** tail, moving* move, int value) {
    if (*head != NULL) {
        while (*head != NULL && (*head)->value == value) del_(head, tail, move, 1);
        
        while (*tail != NULL && (*tail)->value == value) del_(tail, head, move, 0);
        list* tmp_head = *head, *prev = NULL, *next = NULL;
        

        while (*head != NULL) {
            if ((*head)->value == value) {
                (*head)->Xor = XOR((*head)->Xor, prev);

                del_(head, tail, move, 1);
                move->act = prev;
                move->next = *head;

                (*head)->Xor = XOR((*head)->Xor, prev);
                list* tmp_new = *head;
                *head = prev;
                if (*head == tmp_head) tmp_head->Xor = (*head)->Xor = tmp_new;
                else (*head)->Xor = (XOR(XOR((*head)->Xor, next), tmp_new));

                *head = tmp_new;
                next = XOR(prev, (*head)->Xor);
            }
            else {
                next = XOR(prev, (*head)->Xor);
                prev = *head;
                *head = next;
            }
        }
        *head = tmp_head;
    }
}

int main() {
    list *head = NULL, *tail = NULL;
    moving move;
    string arg;
    int val;

    while (cin>>arg) {
        if (arg == "ADD_BEG") {
            cin >> val;
            add_(&head, val, &tail, &move,1);
        }

        else if (arg == "ADD_END") {
            cin >> val;
            add_(&tail, val, &head, &move,0);
        }

        else if (arg == "PRINT_FORWARD") {
            print(head);
        }

        else if (arg == "PRINT_BACKWARD") {
            print(tail);
        }

        else if (arg == "ACTUAL") {
            if (move.act == NULL) cout << "NULL" << endl;
            else cout << move.act->value<<endl;
        }

        else if (arg == "NEXT") {
            next_(&move, head);
            if (move.act == NULL) cout << "NULL" << endl;
            else cout << move.act->value<<endl;
        }

        else if (arg == "PREV") {
            prev_(&move, tail);
            if (move.act == NULL) cout << "NULL" << endl;
            else cout << move.act->value<<endl;
        }

        else if (arg == "DEL_BEG") {
            del_(&head, &tail, &move, 1);
        }

        else if (arg == "DEL_END") {
            del_(&tail, &head, &move, 0);
        }

        else if (arg == "ADD_ACT") {
            cin >> val;
            _act(&head, &tail, &move, val, 1);
        }

        else if (arg == "DEL_ACT") {
            _act(&head, &tail, &move, NULL, 0);
        }

        else if (arg == "DEL_VAL") {
            cin >> val;
            del_val(&head, &tail, &move, val);
        }
    }

    while (head != NULL) {
        del_(&head, &tail, &move, 1);
    }

    return 0;
}