#include <iostream>
#include <ctime>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <utility>
#include <vector>
#include "catch.h"
#include <stdint.h>


std::vector<std::array<uint8_t, 8>> beforeChunks;
std::vector<std::array<uint8_t, 8>> afterChunks;

/**
 * swap function helper for fisher-yates
 * @param a
 * @param b
 */
void swap (uint8_t *a, uint8_t *b)
{
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * fisher-yates will shuffle arrays of uint8 numbers 0-255. when doing this for encryption it will use the previously shuffled array as the input array for the next shuffle.
 * @param arr original array
 * @return shuffled array
 */
std::array<uint8_t, 256> fisherYates (std::array<uint8_t, 256> arr){
    std::array<uint8_t, 256> arrayAfterFY{};// = new std::array<uint8_t, 256>;
    for (int i = 0; i < 256; ++i) {
        arrayAfterFY[i] = arr[i];
    }

    srand (time(nullptr)); //or use clock()
    for (int i = 256 - 1; i > 0; i--){
        int j = rand() % (i + 1);
        swap(&arrayAfterFY[i], &arrayAfterFY[j]);
    }
    return arrayAfterFY;
}

/**
 * takes in a string, breaks it up into 8 char (uint8_t) pieces and adds that new block to the vector of blocks
 * newBlock is an empty char array with all filled to 0. this way, if one string passed in is less than 8 characters, the buffer is already built in and not overwritten.
 * @param chunkString original string passes (will be broken into an 8 character or less string)
 * @param allBlocks vector of all blocks that newBlock is pushed to.
 */
void assignChunk(std::string chunkString, std::vector<std::array<uint8_t, 8>> &allBlocks) {
    std::array<uint8_t, 8> newBlock = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int j = 0; j < chunkString.size(); ++j) {
        newBlock[j] = chunkString.at(j);
    }
    assert(newBlock.size() == 8);
    allBlocks.push_back(newBlock);
}

/**
 * takes in a string and creates blocks of each 8 characters. create a string to manipulate (stringState) out of original string.
 * @param state original plaintext message being brought in.
 * @return vector of uint8_t arrays.
 */
std::vector<std::array<uint8_t, 8>> getBlocks(std::string state) {
    std::vector<std::array<uint8_t, 8>> allBlocks;

    std::string stringState = std::move(state);

    //have giant char array.
    //split that into smaller (8) char arrays.
    for (int i = 0; i < stringState.length();) {
        uint8_t charsLeft = stringState.length() - stringState[i];
        if (charsLeft >= 8) {
            std::string chunk8 = stringState.substr(i, 8);
            assignChunk(chunk8, allBlocks);
//            std::cout << chunk8 << std::endl;
        }
        else {
            std::string chunk8 = stringState.substr(i, charsLeft);
            assignChunk(chunk8, allBlocks);
//            std::cout << chunk8 << std::endl;
        }
        i = i+8;
    }
    beforeChunks = allBlocks;
    return allBlocks;
}

/**
 * this XOR each block with the key;
 * @param state
 * @param key
 * @return
 */
std::array<uint8_t, 8> xorBlocks(std::array<uint8_t, 8> state, std::array<uint8_t, 8> key){
    std::array<uint8_t, 8> stateArray = {0,0,0,0,0,0,0,0};

        for (int i = 0; i < state.size(); ++i) {
            uint8_t xord = state[i] ^ key[i];
            stateArray[i] = xord;
        }
    return stateArray;
}

/**
 * substitutes bytes with the correspding number in the Fisher-Yates table. so if the number from state[1] index was 5 and the number at the 5th index of FY1 was 135, it would change that to 135.
 * @param state block of 8 bytes being passed in
 * @param allFY vector of Fisher-Yates shuffled arrays of 0-255
 * @return state after each byte has been substituted.
 */
std::array<uint8_t, 8> subBytes(std::array<uint8_t, 8> state, std::vector<std::array<uint8_t, 256>> allFY){
    for (int i = 0; i < state.size(); ++i) {
        uint8_t stateInt = state[i]; //56 -177
        uint8_t FYnum = allFY[i].at(stateInt); //177 - 56
        state[i] = FYnum; //177 - 56
    }
    return state;
}

/**
 * same as above, but in reverse
 * @param state
 * @param allFY
 * @return
 */
std::array<uint8_t, 8> subBytesDecrypt(std::array<uint8_t, 8> state, std::vector<std::array<uint8_t, 256>> allFY){
    for (int i = 0; i < state.size(); ++i) {
        uint8_t stateInt = state[i]; //56 -177
        for (int j = 0; j < allFY[i].size(); ++j) {
            if (allFY[i].at(j) == stateInt)
                state[i] = j;
        }
    }
    return state;
}

/**
 * turns a array of uint8 into a uint64. does this by shifting each byte over, one by one, until a uint64 is returned.
 * @param state array passed in
 * @return uint64_t
 */
uint64_t eightToSixtyFour(std::array<uint8_t, 8> state){
    uint64_t fullBytes = 0x00000000;
    for (int i=0; i<8; i++) {
        fullBytes = ((fullBytes << 8) | state[i]);
    }
    return fullBytes;
}

/**
 * same as above but in reverse (for decrypting)
 * @param shiftedNumber
 * @return
 */
std::array<uint8_t, 8> sixtyFourToEight(uint64_t shiftedNumber){
    std::array<uint8_t, 8> shiftedState= {0,0,0,0,0,0,0,0};
    for (int i = 0; i < 8; ++i) {
        shiftedState[i] = uint8_t (shiftedNumber >> 8 * (7 -i) & 0xFF);
    }
    return shiftedState;
}

/**
 * rotates bits in uint64 over one. moves the first bit to the end. first will turn uint8 array into a single uint64. performs the shift, then transforms it back to uint8 array
 * @param state uint8 array being passed in
 * @return uint8 array after shifting
 */
std::array<uint8_t, 8> rotate(std::array<uint8_t, 8> state){
    uint64_t fullBytes = eightToSixtyFour(state);
    uint64_t current = fullBytes;
    uint64_t firstBit = current >> 63;
    uint64_t shiftedBytes = ((current << 1) | firstBit);
    std::array<uint8_t, 8> arrayBack = sixtyFourToEight(shiftedBytes);
    return arrayBack;
}

/**
 * same as above but in reverse
 * @param state
 * @return
 */
std::array<uint8_t, 8> derotate(std::array<uint8_t, 8> state){
    uint64_t fullBytes = eightToSixtyFour(state);
    uint64_t current = fullBytes;
    uint64_t firstBit = current << 63;
    uint64_t shiftedBytes = ((current >> 1) | firstBit);
    return sixtyFourToEight(shiftedBytes);
}

/**
 * encryption using all three methods described above in specific order (XOR, substitute, rotate)
 * @param allBlocks vector of all uint8 array blocks of entire plaintext message
 * @param key to be used in xor
 * @param allFY vector of fisher-yates shuffled arrays of 0-255 numbers for substitution
 * @return encrypted blocks of text
 */
std::vector<std::array<uint8_t, 8>> encryptBlocks(const std::vector<std::array<uint8_t, 8>>& allBlocks, std::array<uint8_t, 8> key, const std::vector<std::array<uint8_t, 256>>& allFY){

    std::vector<std::array<uint8_t, 8>> stateAfterEncrypt{};

    for(std::array<uint8_t, 8> block : allBlocks){
        std::array<uint8_t, 8> stateShuffle = block;
        for (int i = 0; i < 16; ++i) {
            std::array<uint8_t, 8> afterXor = xorBlocks(stateShuffle, key);
            std::array<uint8_t, 8> afterSub = subBytes(afterXor, allFY);
            stateShuffle = rotate(afterSub);
        }
            stateAfterEncrypt.push_back(stateShuffle);
    }
    return stateAfterEncrypt;
}

/**
 * reverse of above.
 * @param allEncryptedBlocks
 * @param key
 * @param allFY
 * @return
 */
std::vector<std::array<uint8_t, 8>> decryptBlocks(const std::vector<std::array<uint8_t, 8>>& allEncryptedBlocks, std::array<uint8_t, 8> key, const std::vector<std::array<uint8_t, 256>>& allFY){

    std::vector<std::array<uint8_t, 8>> stateAfterDecrypt{};

    for(std::array<uint8_t, 8> state : allEncryptedBlocks) {

        std::array<uint8_t, 8> stateShuffle = state;
        for (int i = 0; i < 16; ++i) {
            std::array<uint8_t, 8> afterUnshuffle = derotate(stateShuffle);
            std::array<uint8_t, 8> afterSub = subBytesDecrypt(afterUnshuffle, allFY);
            stateShuffle = xorBlocks(afterSub, key);
        }
            stateAfterDecrypt.push_back(stateShuffle);
    }
    return stateAfterDecrypt;
}


/*=========MAIN=========*/
int main() {
    //std::string state = "eighty8";
    std::string original = "JustinsCryptoPassword";
    int originalSize = original.size();

    char password[originalSize+1];

    strcpy(password, original.c_str());

    std::array<uint8_t, 8> key = {0,0,0,0,0,0,0,0};

    //create key
    for (int i = 0; i < sizeof password; ++i) {
        key[i % 8] = key[i % 8] ^ password[i];
    }

    //create and populate new array for shuffling. array will have values of 0-255.
    std::array<uint8_t, 256> orignalArray{};
    for (int i = 0; i < 256; i++) {
        orignalArray[i] = i;
    }

    //create vector for FY results;
    std::vector<std::array<uint8_t, 256>> allFY{};

    //create tables. will throw away fy0
        std::array<uint8_t, 256> fy0 = fisherYates(orignalArray);
            allFY.push_back(fy0);
        std::array<uint8_t, 256> fy1 = fisherYates(fy0);
            allFY.push_back(fy1);
        std::array<uint8_t, 256> fy2 = fisherYates(fy1);
            allFY.push_back(fy2);
        std::array<uint8_t, 256> fy3 = fisherYates(fy2);
            allFY.push_back(fy3);
        std::array<uint8_t, 256> fy4 = fisherYates(fy3);
            allFY.push_back(fy4);
        std::array<uint8_t, 256> fy5 = fisherYates(fy4);
            allFY.push_back(fy5);
        std::array<uint8_t, 256> fy6 = fisherYates(fy5);
            allFY.push_back(fy6);
        std::array<uint8_t, 256> fy7 = fisherYates(fy6);
            allFY.push_back(fy7);
        std::array<uint8_t, 256> fy8 = fisherYates(fy7);
            allFY.push_back(fy8);

    std::vector<std::array<uint8_t, 8>> stuffBack = getBlocks("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");

    assert(!stuffBack.empty());

    int mistakes = 0;

    int numberOfTestRuns = 10;
    for (int i = 0; i < numberOfTestRuns; ++i) {
    std::vector<std::array<uint8_t, 8>> stateAfterEncrypt = encryptBlocks(stuffBack, key, allFY);

    /*this section shifts one bit. the result is it messes up the whole block.*/
//    stateAfterEncrypt[1][2] = stateAfterEncrypt[1][2]<<1;

    std::vector<std::array<uint8_t, 8>> stateAfterDecrypt = decryptBlocks(stateAfterEncrypt, key, allFY);
    afterChunks = stateAfterDecrypt;

    assert(beforeChunks == afterChunks);
        if (beforeChunks != afterChunks)
            mistakes++;
    }

    for(std::array<uint8_t, 8> arrayOfEights : beforeChunks){
        for(uint8_t c : arrayOfEights){
            std::cout << c << " ";
        }
    }
    std::cout << std::endl;
    for(std::array<uint8_t, 8> arrayOfEights : afterChunks){
        for(uint8_t c : arrayOfEights){
            std::cout << c << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Number of mistakes after " << numberOfTestRuns<< " number of test runs:" << std::endl;
    std::cout << "-------------------" << std::endl;

    std::cout << mistakes << std::endl;

    return 0;
}
