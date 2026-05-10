#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header FIles/car.h"
#include "../Header FIles/helper_functions.h"

#define LICENSE_SIZE 9

//internal functions helpers
// Internal helpers for managing tCar tree nodes
static tCar* insertCarNode(tCar* root, Car* newCar); //Recursively inserts a car nodeף
static Car* searchCarNode(tCar* root, const char* license); // search car by license
static void freeCarNodes(tCar* root);// freeing cars
static tCar* getSuccessor(tCar* node);


//INTERNAL FUNCTIONS
//-------------------
static tCar* insertCarNode(tCar* root, Car* newCar) {
    //checking if new car isn't NULL.
    if (newCar == NULL) {
        return root;
    }
    // in case root doesn't exist it creates one, allocating memory and initializes it.
    if (root == NULL) {
        tCar* newNode = (tCar*)malloc(sizeof(tCar));
        if (newNode == NULL) {
            displayError(1);
            return NULL;
        }
        newNode->p2car = newCar;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode; //returns the newNode in that case it's the new root.
    }
//inserts the Car Node to tree based on license.iterates recursively.
    int cmp = strcmp(newCar->nLicense,root->p2car->nLicense);

    if (cmp < 0)
        root->left = insertCarNode(root->left,newCar);
    else if (cmp > 0)
        root->right = insertCarNode(root->right,newCar);
    else
        printf("the car with license %s already in the system\n", newCar->nLicense);

    return root;
}

static Car* searchCarNode(tCar* root, const char* license) {
    //checks if root exists.
    if (root == NULL) {
        return NULL;
    }
    //iterates recursively in the tree and in every iteration trying to compare between
    //the given license and the current iteration root.
    int cmp = strcmp(license,root->p2car->nLicense);
    if (cmp < 0)
        return searchCarNode(root->left,license);
    else if (cmp > 0)
        return searchCarNode(root->right,license);
    else
        return root->p2car;  //in case found returns the car node.
}

//getting successor if we delete a node in the car tree.
static tCar* getSuccessor(tCar* node) {
    tCar* temp = node->right;
    while (temp && temp->left)
        temp = temp->left;
    return temp;//returns the successor.
}


static void freeCarNodes(tCar* root) {
    if (root != NULL) {
        freeCarNodes(root->left);// freeing the left kid
        freeCarNodes(root->right);//freeing right kid
        free(root->p2car);// freeing the car
        free(root);//freeing the root
    }
}
//////////////////////////////////////
 void saveCarsInOrder(const tCar* root, Car** arr, int* index) {
    if (root == NULL)
        return;
    saveCarsInOrder(root->left, arr, index);
    arr[*index] = root->p2car;
    (*index)++;
    saveCarsInOrder(root->right, arr, index);
}

//comparing by highest payed
int compareByPayed(const void* a, const void* b) {
    if (a == NULL || b == NULL)
        return 0;
    const Car* c1 = *(const Car**)a;
    const Car* c2 = *(const Car**)b;
    if (c2->totalPayed > c1->totalPayed)return 1;
    else if( c2->totalPayed < c1->totalPayed)return -1;
    return 0;
}


Car* createCar(const char* license, int portType) {
//license must be 8 digits long
    if (strlen(license) != 8) {
        displayError(19);
        return NULL;
    }
    //if invalid port type returning NULL
    if (portType != FAST && portType != MID && portType != SLOW) {
        displayError(14);
        return NULL;
    }
    // allocate memory for the new car.
    Car* newCar = calloc(1,sizeof(Car));
    if (newCar == NULL) {
        displayError(1);
        return NULL;
    }
    //copies the car information to the new car.
    strcpy(newCar->nLicense,license);
    newCar->portType = portType;
    newCar->totalPayed = 0.0;
    newCar->pPort = NULL;
    newCar->inqueue = 0;
    return newCar; //return the new car
}
//prints the license of the car and the total payment.
void printCar(const Car* c) {
    printf("Car, %s\n", c->nLicense);
    printf("total payed:%.2f\n",c->totalPayed);
}

int countCarNodes(const tCar* root) {
    if (root == NULL)
        return 0;
//counting recursively. every node adds 1.
    return 1 + countCarNodes(root->left) + countCarNodes(root->right);
}

//counting cars using recursive
int countCars(const CarBST* CarTree){
    if (CarTree == NULL) return 0;
    return countCarNodes(CarTree->root);
}


int isCarAvailableForDeletion(const Car* c) {
    if (c == NULL) //if car doesn't exist return 0;
        return 0;
    if (c->pPort != NULL) // if the car pointing to port(means its charging) return 0.
        return 0;
    if (c->inqueue == 1) // if car is in the queue return 0.
        return 0;

    return 1; //returns 1 if available for deletion.
}

tCar* removeCarNode(tCar* node, const char* license) {
    if (node == NULL) //node doesn't exist return NULL.
        return node;
//comparing given license with the node car license.
    int cmp = strcmp(license,node->p2car->nLicense);
    if (cmp < 0)
        node->left = removeCarNode(node->left, license);
    else if (cmp > 0){
        node->right = removeCarNode(node->right, license);
    }
    //when the comparing succeeded, checking if car available.
    //available - not in queue and not in charge.
    else {
        if (!isCarAvailableForDeletion(node->p2car)) {
            printf("car %s is waiting in line or charging\n", node->p2car->nLicense);
            return node;
        }

        //in case car is available.
        //-------------------------
        //if there is no left kid.
        if (node->left == NULL) {
            tCar* temp = node->right; //saving the right kid to return.
            //releasing the car struct and the node.
            free(node->p2car);
            free(node);
            return temp;// returning the kid to continue connect to the tree.
        //if there is no right kid.
        }else if ( node->right == NULL) {
            tCar* temp = node->left; //saving the left kid to return.
            //as mentioned above
            free(node->p2car);
            free(node);
            return temp;
        }

        //if there is two kids, we find the successor.
        tCar* succ = getSuccessor(node);
        tCar* tempRight = node->right;
        tCar* tempLeft = node->left;

        //removing the node where the successor is in from the right kid.
        tempRight = removeCarNode(tempRight, succ->p2car->nLicense);

        //connecting the kids to the successor that replace the current node.
        succ->left = tempLeft;
        succ->right = tempRight;

        //freeing the car struct and the original given node,
        //and returning the successor that will take the original node location in the tree.
        free(node->p2car);
        free(node);
        return succ;
    }
    //in case the given car isn't available returning the current node.
    return node;
}


CarBST* createCarBST() {
    //allocating memory for new binary tree
    //and initialize the field.
    CarBST* CarTree = (CarBST*)malloc(sizeof(CarBST));
    if (!CarTree) {
        displayError(1);
        return NULL;
    }
    CarTree->root = NULL;
    CarTree->totalCars = 0;
    return CarTree;//returning the new tree.
}

int insertCar(CarBST* CarTree, Car* newCar) {
    if (!CarTree || !newCar) return 0;
    if (searchCar(CarTree, newCar->nLicense) != NULL) {
        printf("Car already in the system\n");
        return 0;
    }
    CarTree->root = insertCarNode(CarTree->root, newCar); //running on the tree recursively.
    //update the totalCars field in the tree ADT when inserting a car.
    CarTree->totalCars++;
    return 1;// returning 1 if succeeded.
}

Car* searchCar(CarBST* CarTree, const char* license) {
    if (!CarTree || !license) return NULL;
    return searchCarNode(CarTree->root, license); //search car recursively
}


void freeCarsTree(CarBST* CarTree) {
    if (!CarTree) return;
    //freeing all the car nodes in the tree and after that free the tree itself.
    freeCarNodes(CarTree->root);
    free(CarTree);
}


int totalChargeMinutes(Car* car, Date now_time) {
    int startMinutes = DateToMinutes(car->pPort->tin);
    int currentMinutes = DateToMinutes(now_time);

    return currentMinutes - startMinutes;
    // the difference between the current time and the initial charging time of a car in minutes.
}
