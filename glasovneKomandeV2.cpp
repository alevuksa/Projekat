#include <iostream>
#include <pocketsphinx.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/err.h>


const char *hotword = "telefon";
const char *commands[] = {"otvori kalkulator", "otvori beleške", "otvori internet", "ugasi"};


bool perform_action(const char *command) {
    
    if (strcmp(command, "otvori kalkulator") == 0) {
        system("calc.exe");
        std::cout << "Otvaram kalkulator..." << std::endl;
    } else if (strcmp(command, "otvori beleške") == 0) {
        system("notepad.exe");
        std::cout << "Otvaram beleške..." << std::endl;
    } else if (strcmp(command, "otvori internet") == 0) {
        system("start chrome");
        std::cout << "Otvaram internet..." << std::endl;
    } else if (strcmp(command, "ugasi") == 0) {
        std::cout << "Gasim program..." << std::endl;
        return false;
    } else {
        std::cout << "Nepoznata komanda." << std::endl;
    }
    return true;
}


bool voice_command_recognition() {
    std::cout << "Slušam..." << std::endl;


    cmd_ln_t *config = cmd_ln_init(nullptr, ps_args(), TRUE,
        "-hmm", "path/to/acoustic/model",
        "-lm", "path/to/language/model",
        "-dict", "path/to/dictionary",
        nullptr);

    
    ps_decoder_t *decoder = ps_init(config);

    ad_rec_t *mic = ad_open_dev(nullptr, default_sampling_rate, default_samples_per_sec);

    
    ad_start_rec(mic);

   
    int16 buf[4096];
    int32 num_frames;
    int32 max_frames = 500;  
    ad_read(mic, buf, 4096);
    ps_start_utt(decoder);
    ps_process_raw(decoder, buf, 4096, false, false);
    num_frames = ps_get_n_frames(decoder);
    while (num_frames < max_frames) {
        ad_read(mic, buf, 4096);
        ps_process_raw(decoder, buf, 4096, false, false);
        num_frames = ps_get_n_frames(decoder);
    }
    ad_stop_rec(mic);
    ps_end_utt(decoder);

  
    const char *text = ps_get_hyp(decoder, nullptr);
    std::cout << "Rekli ste: " << text << std::endl;

  
    if (strstr(text, hotword) != nullptr) {
       
        char *command = strcasestr(text, hotword);
        memmove(command, command + strlen(hotword), strlen(command + strlen(hotword)) + 1);

      
        bool recognized = false;
    }}