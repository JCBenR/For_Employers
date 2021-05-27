#include <iostream>
#include <array>
#include <vector>


std::string key = "JustinsCryptoKey";
std::string key2 = "JustinsCryptoKey2";

/**
 * makes permutation of numbers 0-255
 * @return array of numbers populated
 */
std::array<uint8_t, 256> makeArrayOfNumbers(){
    std::array<uint8_t, 256> arrayOfNumbers{};
    for (int i = 0; i < 256; i++) {
        arrayOfNumbers[i] = i;
    }
    return arrayOfNumbers;
}

/**
 * swap function helper
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
 * method for initializing the array
 */
std::array<uint8_t, 256> ronShuffle(std::string key){
    std::array<uint8_t, 256> numArray = makeArrayOfNumbers();
    uint8_t j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j+numArray[i] + key[i % key.length()]) % 256; //might need to cast key to uint8_t
        swap(&numArray[i], &numArray[j]);
    }
    return numArray;
}

/**
 * DEPRECIATED generates next byte to be XOR'd with character
 * @param initalizedArray shuffled initialized array
 * @param i counter
 * @param j counter
 * @return byte to XOR with character
 */
//uint8_t nextByte(std::array<uint8_t, 256> initalizedArray, uint8_t i, uint8_t j){
//    i = i++ % 256;
//    j += (initalizedArray[i]) % 256;
//    swap(&initalizedArray[i], &initalizedArray[j]);
//    return initalizedArray[(initalizedArray[i] + initalizedArray[j]) % 256];
//}

/**
 * encrypts message. initializes a shuffled array
 * performs encryption on each character
 * @param message plaintext message to be encrypted
 * @return encrypted version of message
 */
std::string encryptWithByte( std::string message){
    std::array<uint8_t, 256> initalizedArray = ronShuffle(key);
    uint8_t i = 0;
    uint8_t j = 0;
    std::string finalString;
    for(char c : message) {
        i = i+1 % 256;
        j += (initalizedArray[i]) % 256;
        swap(&initalizedArray[i], &initalizedArray[j]);
        uint8_t addedByte = initalizedArray[(initalizedArray[i] + initalizedArray[j]) % 256];
        finalString.push_back( c ^ addedByte);
    }
    return finalString;
}

int main() {
    std::string plaintext = "helloThisIsJustin";
    std::string plaintext2 = "secondPlainTextMessage";
    std::string plaintext3 = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    std::string thousand = "Your salary is $1000";
    std::string tenThousand = "Your salary is $9999";
    std::string hundred =  "Yours salary is $100";
    std::string kirksTest = "Modify part of a message using a bitflipping attack. For example, try sending the message Your salary is 1000 encrypted with RC4.";


    //KEYS
    std::array<uint8_t, 256> initalizedArray = ronShuffle(key); //key 1
    std::array<uint8_t, 256> initalizedArray2 = ronShuffle(key2); //key 2 (for testing they produce different results)

    assert(initalizedArray != initalizedArray2);


    std::string encryptedText = encryptWithByte(thousand);

    std::string encryptedText2=encryptWithByte( thousand);

    std::string decryptedText=encryptWithByte(encryptedText);


    //COMPARING TWO MESSAGES WITH THE SAME KEY
    std::string decryptedXorText{};
    for (int k = 0; k < encryptedText.size(); ++k) {
        decryptedXorText.push_back(encryptedText[k] ^ encryptedText2[k]);
    }

    //"Your salary is $1000";
    //"Yours salary is $100";
    //     ^^^^^^^^^^^^^^
    // 12345678901234567890

//    std::cout << "xord" << std::endl;
//    for(char c : decryptedXorText)
//        std::cout << +c << " ";
//    std::cout <<  std::endl;

/*
 * this seems to give back bits and pieces of the two messages where they differ. it also can tell a lot about the structure of the message. what i got back was the int which was the difference between subtracting one char from anothers ascii value.
 *
 * */
//    std::cout << "123456789012345678901234567890" << std::endl;


    //END



    //00001111101000
    //10011100001111
    //0001 0000 0000 0000
    //1001 1001 1001 1001

    /*BITFLIPPING ATTACK*/

    std::string bitflipXorOne{};
    bitflipXorOne.reserve(encryptedText.size());
    for (int k = 0; k < encryptedText.size(); ++k) {
        bitflipXorOne.push_back(encryptedText[k] ^ thousand[k]);
    }

    std::string bitflipXorTwo{};
    bitflipXorTwo.reserve(bitflipXorOne.size());
    for (int k = 0; k < bitflipXorOne.size(); ++k) {
        bitflipXorTwo.push_back(bitflipXorOne[k] ^ tenThousand[k]);
    }

    std::string decrypted10K=encryptWithByte(bitflipXorTwo);

    std::cout << "encrypted text" <<std::endl;

    for(char c : encryptedText)
        std::cout << +c << " ";

    std::cout << std::endl;

//    assert(decryptedText == plaintext);
    std::cout << "decrypted text" <<std::endl;

    for(char c : decryptedText)
        std::cout << c << " ";

    std::cout << std::endl;
    std::cout << "bitflipped version" <<std::endl;

    for(char c : decrypted10K)
        std::cout << c << " ";


    return 0;
}
