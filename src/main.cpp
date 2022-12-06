#include <iostream>
#include <string>
#include "Adfgvx.h"
#include "Enigma.h"
#include "Playfair.h"
#include "ViginerePlus.h"
#include <chrono>
#include <memory>
#include <complex>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// argv[0] holds the name of the program.
// argv[1] points to the first command line argument and argv[n] points last argument.
int main(int argc, char *argv[]) {

    std::string encr_algo;
//    std::string test;
//
//    std::ifstream f("../data/test.txt"); //taking file as inputstream
//    if (f) {
//        std::ostringstream ss;
//        ss << f.rdbuf(); // reading data
//        test = ss.str();
//    }
//    std::vector<double> freq;
//    AlgorithmDecryption::getAlphabetFrequencies(test,freq);
//    std::cout << "a " << std::endl;

//    for (auto freq : LETTER_FREQUENCIES){
//        float sum = 0;
//        AlgorithmDecryption::normalize(freq.second);
//        std::cout << freq.first << ":" << std::endl << '\t' ;
//        for (float elem : freq.second){
//            sum += elem*elem;
//            std::cout<< elem << ", ";
//        }
//
//        std::cout << std::endl << "sum: " << std::sqrt(sum) << std::endl;
//
//    }
    if(argc>=2) {
        encr_algo = argv[1];
    }
    auto start = high_resolution_clock::now();
    std::shared_ptr<AlgorithmDecryption> algorithmDecryption;

    if (encr_algo.empty()) { // all encryption algorithms
//        algorithmDecryption = std::make_shared<ViginerePlus>(ViginerePlus("../data/01-OPGAVE-vigenerePlus.txt"));
    }
    else if ( encr_algo == "VP"){ // specific encryption algorithm
//        std::string ciphertext = "CSLKSMAQTEEZZVCAHVCBLGDIKNPIUDEIPVSYZIKEQRITOKTWRNLSJTVXJLIINPEKYSRYLWVHRZYOEUAHFFOLJFPEZBMULDREASOLBFOGAJVHROAYFKEEWGERWERCMHQUTYPXPCAFFCUXJHTJZUIDVDPHIWCUOKSMVYVMRYBLGMIIDBXJPNXTVSVPCVOIFQBTKSQWYVMRYEEUOEIDQDGZHVHIWVPNPAZSDHBCJVSOVRVEPEPMOLCIRFHHRWNJGLTKLTPUOECZWOGKQLTBIAVUERQKWLSJPLEDVUKEEIPAYWTDIQYSZIIRFZHVHIWXLRPAZIVAYJSMLCKOELZEVOEIDBCNPSYWWRIILRNSHTLSJEPEVYERNPIFYIXSBXQAHVRZSWUDRYLWJLWFCMFNHCBRTSXLSKSIXEHMVFXXQOEIPTFQDSLYTMMLTYPWXJLRJDPIYHSEEEICYIERILCASYPLMFUTCZWOVVMVWQOGHWZEKLCAACWJYVZHVNWYNKNKAWWUPBCJVSVIEFYMSVOEIHQWGDHREWRGHRKSEEUZHVOWMPNUGEPITLOEEPIRSAKQWVOHNUHPCHVRYPIZGUSJLSIYLRVLTPVOEFEPITDIKNPIUNAQTVKCAHVCEMVOSLNPEOPXKFZIQMAUZZEVPOELEICUDWPIVXLRPDTSYSYKSMCQBNXWIHAVNKSMTNHTWZZQTHIJPLLGYHRYLWVVHVCNEELIJLELGYGCZDIFMIERMVUBNYZWOKUGJZUIVOIERJIJPNUSMVGHRJLVHVOEEEPIPZHVNIYIOTYZTHQMHVCKLGLKJLVHNPFKPLLGYFRNMGNLAELEEAAHVHPSNLOWEPEVWRVEBCHHCVNIQGHWRJQRJLRYLVHUPTNLAEOHSBLAWJLTFZSSHMTYPUEURSYPBYTUEUDQHGDAPDIRFWLRNMHKACRCMJWSLPFXSPHSDLTPVHBCPVICYBPLVHYOEEDPIVBREPLVQBNULOEKUAEONEELDLDQZGYYEPIVNFSTCMEOLDFFBPQBDKSIXHHCVZNLGYSNLAXJLMFDBJTPGYENYNHNUQZMIOTVYQRIAHZYOMJHVVPDITZEVYRYUALFZSMPNAKTBKCCEDPBLGZHRVMWCSLFGMVKAWRDASEYUDATIFHNUHQDGUEUDWWJYUEVMRCUDJSZMXLLCPLMVSOFVMHCZTYZCKJPTYLLFGLNGTKONLDZYDMPLGRCQXYHSRQMETZODPIRFNHRDBPAZIXSBXJLRVHIWUVMVEPMPNTVCZMDSYNCWRIDIKSQXUVMVEPMPNFFFTEPKPLEZMFHNUOMGCFEUTBWGLMVOYYKAECTBITHLCJBSDLRFEBMPNANLGEVAHVPLKGZAEOQRVOEDTLHNLOWEPIHHCVLZSWUDKSMQQBTYLVHEOEVVAMEVUCOAIGAHVDSMPHLCNIRMLRVOIRFDOIXMEVLNRDBLQBGYXIKIVTJHMVGDOIVQRIHWRJQRVOEIPBLGYERCMXKTEJHPIPZODPBLKUGZDASHYIXSBJWSYFFJIEVMVXMWOLRZDMHDFIKLVHEHNKWWSMHWRJQACZLZVMXJHTEZEMYHSKCIRUMIOPLMYHSEFUFGKINLAQCNNVEQWGKBPEPIUOEVCPSTYOIZNXJPSNZUEPZFVLBYTLSSFBXJLRVHIWOVRVEWMVAHRYBLCATYPZIYHSRWWSMVFJPZTGUTJTVXJVSVPGIUVFYPZWCZTYPGJNHSYPLETVUEOBLGHUUTMRELIBYMAKTMVOQEVLLPZNGQBRJPBLCATYTAACZNFYMSVOEIEPEPAHVRZEPKHZRPAKACYSMVULLWTSRGDACDWAJFSYPPEFDOIYIQCZKJSMGQBLUYMZGYHRGMQQCEULZSWUDZYXYDSITWMXCSOEPJSQRIELBEPOOKPTAKAHYPZVGHLWLKIGCEIJWRGDHFDIAJLRNZCPFOAMPZYPHWRJAGTLADTVKVOEUZWVUZHFFBIFAHVRZEPKHZRPAKACYTVEXVITPBLCAFZWTIFAHVCWSOHNUMWYPJEULZSWUDKSMACSLJLZIVOEPNPEKUEULVHDVLKPLXJLDFZZWCYETSIMPLDRYLFQSTVOGSWYGILVHPLSJLVWYLRVOIZQPCVTVXJLALOQIPJEKSMFTPLCTIRVZNRVMWGFEJEPEVDEIPAIVZOUPMTKUTYLBHTLAUQCPTVTKTVKYVRDPIXGUFRNMKNHRVOCRDSIEVQRISYREBLGDIKNPIUDHFDIXHHCZYOLGYYFFUEAYRIPMQQVFPZCVISOMPAWJLSYZCXGKHVCDSKJEZYWXKJEUSIHVOAKDIQGOAIOUIVHLCTKUWHLZEGEUAHVGWMELOWEPIYPTTSQLCKMVECRFLRKSMGQUKVCBVGLOEWGMVDAJQIVNVUUPZEPKMLNPQWJHYLZWJLRZEZEUWEUTBKTHTVOQXUUAIWMHKASTCITGKIKDPVKLKVOIRFPTXCWANLDVGMVAVNVTVXJLRFZUACZPVPTMPNOWQPITNLFGMWKDAJHIXEOIERBLGOAEOASHAHFDMMPAHVMIGMYONTEEPAEUGMVATUTSBSULENSIXVOEZCNMPNEIDTSQREUWQOGHNUHPIVOEIXGKTHNUXWXJLRYLLFGLNITOLVHHPPAMEVUCOAIGZEMPZENVFKSMQPVWZNWYNKSVPBLGIRFHVGNHWJNCVXPNXZDITAHVEQTUVFKSMJKUGVCAXJLYNPZICIOLEBAQPNTSMWNVNXEPSULCCLEWCUDJSIVRHTKSMIPKS";
//        std::string key = "CHARLIE";
//        std::cout << ViginerePlus::decrypt(
//                ciphertext,key) << std::endl;
        algorithmDecryption = std::make_shared<ViginerePlus>(ViginerePlus("../data/01-OPGAVE-vigenerePlus.txt"));

    }
    else if ( encr_algo == "PF"){ // specific encryption algorithm
        algorithmDecryption = std::make_shared<Playfair>(Playfair("../data/02-OPGAVE-playfair.txt"));
    }
    else if (encr_algo == "AD") {
        algorithmDecryption = std::make_shared<Adfgvx>(Adfgvx("../data/03-OPGAVE-adfgvx.txt"));
    }
    else if (encr_algo == "EN") {
        algorithmDecryption = std::make_shared<Enigma>(Enigma("../data/04-OPGAVE-enigma.txt"));

    }
    std::string out = algorithmDecryption->Solve();
    std::cout << out << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Time taken by main: " << duration.count() << " seconds" << endl;
    return 0;
}
