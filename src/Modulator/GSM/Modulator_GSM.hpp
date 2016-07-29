#ifndef MODULATOR_GSM_HPP_
#define MODULATOR_GSM_HPP_


#include <vector>
#include "../../Tools/MIPP/mipp.h"
#include "../../Tools/params.h"
#include "../../Tools/Math/max.h"

#include "../Modulator.hpp"

// Trick to allow right size construction
#define UP_SAMPLE_FACTOR    5
#define M_ORDER             2
#define NB_OUTPUT_SYMBS     16
#define NB_STATES           8
#define NB_BITS_PER_SYMP    1
#define NB_PREV_BRANCHES    2

template <typename B, typename R, typename Q, proto_max<Q> MAX>
class Modulator_GSM : public Modulator<B,R,Q>
{
protected:
    const R sigma;
    
public:
    Modulator_GSM(int N, const R sigma = 1.0);
    virtual ~Modulator_GSM();

    void   modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2); //const; // Changed To allow Coset Approach in IterDEV
    void demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2); //const; // Changed To allow Coset Approach in IterDEV
    void     filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2); //const; // Changed To allow Coset Approach in IterDEV

    /* Modulate Data Struct */
    // TODO
    mipp::vector<B> parity_enc;
    mipp::vector<R> wave_enc;

    /* Demodulate Data Struct */
    mipp::vector<Q> L_a_cpm;
    mipp::vector<Q> L_ext_cpm;
    mipp::vector<Q> proba_msg_symb;
    mipp::vector<Q> proba_msg_bits;
    mipp::vector<Q> filtered_signal_r; 

    /* Data Structures for BCJR Processing */
    mipp::vector<Q> symb_apriori_prob;
    mipp::vector<Q> BCJR_gamma;
    mipp::vector<Q> BCJR_alpha;
    mipp::vector<Q> BCJR_beta;
    mipp::vector<Q> log_bits_probability;

    int get_buffer_size_after_modulation(const int N);
    int get_buffer_size_after_filtering(const int N);


    //
    //  BCJR FUNCTIONS FOR DEMODULATION ////////////////////////////////////////////////////////////////////////////////
    //

    void BCJR_decode(const mipp::vector<Q> &llr,            const Q noise_var,                     
                     const mipp::vector<Q> &signal,
                           mipp::vector<Q> &proba_msg_symb, mipp::vector<Q> &proba_msg_bits, 
                           mipp::vector<Q> &L_ext); //const; // Changed To allow Coset Approach in IterDEV

    /* Math Functions */
    void matCProd(  /* Inputs */    int rowNb1, int columnNb1, R *mat1_r, R *mat1_i, 
                                    int rowNb2, int columnNb2, R *mat2_r, R *mat2_i, 
                    /* Outputs */   R *output_r, R *output_i); //const; // Changed To allow Coset Approach in IterDEV

    void R_matCProd(    /* Inputs */    int rowNb1, int columnNb1, R *mat1_r, R *mat1_i, 
                                        int rowNb2, int columnNb2, R *mat2_r, R *mat2_i, 
                        /* Outputs */   mipp::vector<R> &output_r); //const; // Changed To allow Coset Approach in IterDEV
    //
    //  BCJR CONSTANTS FOR DEMODULATION ////////////////////////////////////////////////////////////////////////////////////
    //

    /* Needed Const: */
    const int up_sample_factor  = UP_SAMPLE_FACTOR;
    const int nb_output_symbs   = NB_OUTPUT_SYMBS;
    const int nb_states         = NB_STATES;
    const int m_order           = M_ORDER;
    const int nb_bits_per_symb  = NB_BITS_PER_SYMP;
    const int nb_prev_branches  = NB_PREV_BRANCHES;

    /* Translation of Base Band Vectors: */
    const float baseband_vector_r[80] = {
    -0.00564198672890523552497787918014, 0.00564198672890511322697282281524, 
    -0.52091994211246372969981166534126, 0.52091994211246384072211412785691, 
    -0.52091994211246317458829935276299, 0.52091994211246339663290427779430, 
    -0.00564198672890548098834972989835, 0.00564198672890560328635478626325, 
    -0.00564198672890523552497787918014, 0.00564198672890511322697282281524, 
    -0.52091994211246372969981166534126, 0.52091994211246384072211412785691, 
    -0.52091994211246317458829935276299, 0.52091994211246339663290427779430, 
    -0.00564198672890548098834972989835, 0.00564198672890560328635478626325, 
    -0.00776877723203855691941344119300, 0.00776877723203887871061823489072, 
    -0.28757227949570324243566687982820, 0.28757227949570335345796934234386, 
    -0.78729088687072712460235379694495, 0.78729088687072712460235379694495, 
    0.57035903678899979585281698746257, -0.57035903678899968483051452494692, 
    -0.02137400816179174656284089905967, 0.02137400816179184717680250571448, 
    -0.27451404512079868869278698184644, 0.27451404512079879971508944436209, 
    -0.79560744046001319329519674283802, 0.79560744046001286022828935529105, 
    0.58148244165252382931186048153904, -0.58148244165252371828955801902339, 
    -0.00834221760492034544731687617514, 0.00834221760492066810588340786126, 
    -0.13452830303649640497631878588436, 0.13452830303649651599862124840001, 
    -0.97383659537451217058645625002100, 0.97383659537451217058645625002100, 
    0.93140875618689789039450488417060, -0.93140875618689789039450488417060, 
    -0.05833956743729826893707368640207, 0.05833956743729836608158834110327, 
    -0.08480504399984668817413790975479, 0.08480504399984681307422818008490, 
    -0.98398270811805732538601887426921, 0.98398270811805732538601887426921, 
    0.94844553546340304439610235931468, -0.94844553546340293337379989679903, 
    -0.00858018935547101568861450004988, 0.00858018935547133661245755575919, 
    -0.05270696031830793965022863289960, 0.05270696031830806455031890322971, 
    -0.97253937780659382994485895324033, 0.97253937780659382994485895324033, 
    0.98497264802923345516916242559091, -0.98497264802923345516916242559091, 
    -0.14011593487553636316889082991111, 0.14011593487553603010198344236414, 
    0.07918383214916917267611751185541, -0.07918383214916904777602724152530, 
    -0.93344722234109755998332502713311, 0.93344722234109778202792995216441, 
    0.95367293175296929952367008809233, -0.95367293175296929952367008809233, 
    -0.00915443117019493039776545373343, 0.00915443117019525305633198541955, 
    -0.01573363938574873222009387063736, 0.01573363938574885365073718901385, 
    -0.78380065831311762636346429644618, 0.78380065831311740431885937141487, 
    0.79901259618196074363538627949310, -0.79901259618195974443466411685222, 
    -0.29297109943652965835525492366287, 0.29297109943653021346676723624114, 
    0.26908606242929095087035307187762, -0.26908606242929083984805060936196, 
    -0.57498371378513679630373189866077, 0.57498371378513690732603436117643, 
    0.59516671290393219351244624704123, -0.59516671290393230453474870955688
    };
    const float baseband_vector_i[80] = {
    0.99998408386621373544755897455616, -0.99998408386621373544755897455616, 
    -0.85360553765164115525010402052430, 0.85360553765164115525010402052430, 
    0.85360553765164148831701140807127, -0.85360553765164137729470894555561, 
    -0.99998408386621373544755897455616, 0.99998408386621373544755897455616, 
    0.99998408386621373544755897455616, -0.99998408386621373544755897455616, 
    -0.85360553765164115525010402052430, 0.85360553765164115525010402052430, 
    0.85360553765164148831701140807127, -0.85360553765164137729470894555561, 
    -0.99998408386621373544755897455616, 0.99998408386621373544755897455616, 
    0.99996982259482158905683490957017, -0.99996982259482158905683490957017, 
    -0.95775893838984615946685607923428, 0.95775893838984615946685607923428, 
    0.61658175406859383471669389109593, -0.61658175406859394573899635361158, 
    -0.82139550105483560837882350824657, 0.82139550105483560837882350824657, 
    0.99977154979280125068896722950740, -0.99977154979280125068896722950740, 
    -0.96158309002988195945960114841000, 0.96158309002988195945960114841000, 
    0.60581251281619008519641056409455, -0.60581251281619052928562041415717, 
    -0.81355895302665015567100681437296, 0.81355895302665026669330927688861, 
    0.99996520309730385633883997797966, -0.99996520309730385633883997797966, 
    -0.99090975153245952622427239475655, 0.99090975153245941520196993224090, 
    0.22724939055887161121205508607090, -0.22724939055887172223435754858656, 
    -0.36397490146763655793904490565183, 0.36397490146763666896134736816748, 
    0.99829679698535989107455179691897, -0.99829679698535989107455179691897, 
    -0.99639756348165764432422975005466, 0.99639756348165764432422975005466, 
    0.17826393388639785730909181893367, -0.17826393388639799608696989707823, 
    -0.31694016195417507031351078694570, 0.31694016195417518133581324946135, 
    0.99996318949780560458862055384088, -0.99996318949780560458862055384088, 
    -0.99861002214778737950950926460791, 0.99861002214778737950950926460791, 
    -0.23273839093618237106042556661123, 0.23273839093618267637175733852928, 
    0.17271040105992410684798699094245, -0.17271040105992399582568452842679, 
    0.99013510431352469431942608935060, -0.99013510431352480534172855186625, 
    -0.99686003065935602229785672534490, 0.99686003065935602229785672534490, 
    -0.35871476566164589572238696746354, 0.35871476566164534061087465488527, 
    0.30084537430663010892928355133336, -0.30084537430662999790698108881770, 
    0.99995809731705764811238168476848, -0.99995809731705764811238168476848, 
    -0.99987621863492648355276060101460, 0.99987621863492648355276060101460, 
    -0.62101250231208993124454309509019, 0.62101250231209015328914802012150, 
    0.60131428649464413460634659713833, -0.60131428649464535585167368481052, 
    0.95612129716629101761071751752752, -0.95612129716629090658841505501186, 
    -0.96311613578337462726608464436140, 0.96311613578337462726608464436140, 
    -0.81816485434284691447004433939583, 0.81816485434284680344774187688017, 
    0.80360225475736957179151431773789, -0.80360225475736957179151431773789
    };

    /* Translation of Filtering Generator Familly: */
    const float proj_r[80] = {
    -0.00132029697684193014425857448657, -0.00058505123686783551927026447714, 
    -0.00339468397069032836593782676005, -0.00040650650234961400140976106599, 
    -0.00219078471377524104310441366295, 0.00132029697684190217184252436056, 
    0.00058505123686791336498624893636, 0.00339468397069037433610994014543, 
    0.00040650650234968924504053155999, 0.00219078471377530305946867983380, 
    -0.10401748554092599130527219131181, -0.05837574642016117587584034254178, 
    -0.02534602518731315576316909243815, -0.01173906302348584791694907636384, 
    -0.00281456991227095219484932719922, 0.10401748554092603293863561475519, 
    0.05837574642016116893694643863455, 0.02534602518731318004929775611345, 
    0.01173906302348587740724816796956, 0.00281456991227097778202059785713, 
    -0.10419838646349323385553731213804, -0.15737182038912753134773936380952, 
    -0.19493752797443592017501146074210, -0.19436919790676290831221706412180, 
    -0.15680021941424224229955086684640, 0.10419838646349324773332511995250, 
    0.15737182038912755910331497943844, 0.19493752797443592017501146074210, 
    0.19436919790676288055664144849288, 0.15680021941424218678839963558858, 
    -0.00110369058882891146397553505665, 0.11406293921541321711998762111762, 
    0.18615930726484100632411866627081, 0.19716837111164342566382856603013, 
    0.15973801222277886346923025939759, 0.00110369058882893249749768127543, 
    -0.11406293921541320324219981330316, -0.18615930726484103407969428189972, 
    -0.19716837111164342566382856603013, -0.15973801222277864142462533436628, 
    -0.00141366279319429452576706296441, -0.00285994240979238478761459418820, 
    -0.01415919085618501035983562985621, -0.02615087290929190427801742657721, 
    -0.05910515553292910312377372861192, 0.00141366279319428541846881408617, 
    0.00285994240979233101118683890718, 0.01415919085618514219881980409355, 
    0.02615087290929176550013934843264, 0.05910515553292924190165180675649, 
    -0.10392385970452529264651531093477, -0.05622480885151811408340094544656, 
    -0.01459459897167380768023825510227, 0.01403539744041460562984369175865, 
    0.05431356462776571608630504783832, 0.10392385970452532040209092656369, 
    0.05622480885151812102229484935378, 0.01459459897167383370109039475437, 
    -0.01403539744041458481316198003697, -0.05431356462776569526962333611664, 
    -0.10381139199392126759580179395925, -0.16067748314182234148361771985947, 
    -0.19441140682922075644256665327703, -0.18827180589878331207209782860446, 
    -0.11460877603955207937325155853614, 0.10381139199392130922916521740262, 
    0.16067748314182225821689087297273, 0.19441140682922078419814226890594, 
    0.18827180589878333982767344423337, 0.11460877603955207937325155853614, 
    -0.00148861390352549114585167799873, 0.11790671198601079816548065082316, 
    0.18706804997001283408231131488719, 0.19257748293331786171300734622491, 
    0.11855691706085519365743152775394, 0.00148861390352552540664032854068, 
    -0.11790671198601074265432941956533, -0.18706804997001277857116008362937, 
    -0.19257748293331783395743173059600, -0.11855691706085520753521933556840
    };
    const float proj_i[80] = {
    -0.20022994699815499508588345634053, -0.19930212538608207961665641505533, 
    -0.20064714980915868136612800753937, -0.19983533762996610883000414560229, 
    -0.19996481148887482182097130589682, 0.20022994699815499508588345634053, 
    0.19930212538608207961665641505533, 0.20064714980915868136612800753937, 
    0.19983533762996610883000414560229, 0.19996481148887482182097130589682, 
    0.17094374945445667268728584531345, 0.19087396185449426178060150505189, 
    0.19885754516415377057825253359624, 0.19952120033548864652672705233272, 
    0.19996601402686037252109940709488, -0.17094374945445667268728584531345, 
    -0.19087396185449428953617712068080, -0.19885754516415371506710130233841, 
    -0.19952120033548867428230266796163, -0.19996601402686037252109940709488, 
    -0.17075199660761980480216948308225, -0.12321342678018226690639380649372, 
    -0.04557004002377239987309209823252, 0.04660179542834700511244605536376, 
    0.12419528167265808660246761974122, 0.17075199660761977704659386745334, 
    0.12321342678018232241754503775155, 0.04557004002377246926203113730480, 
    -0.04660179542834712307364242178664, -0.12419528167265811435804323537013, 
    0.19991334030230489826607254144619, 0.16462218757988547612747254333954, 
    0.07227703165261167850275114687975, -0.03420387753798159041762616539017, 
    -0.12034449819846976259540838327666, -0.19991334030230492602164815707511, 
    -0.16462218757988550388304815896845, -0.07227703165261160911381210780746, 
    0.03420387753798150715089931850343, 0.12034449819847002627337673175134, 
    -0.20031873203795824700534922158113, -0.19901969626463703044727537871950, 
    -0.20050097565137225963383116322802, -0.19787101498833303825364282602095, 
    -0.19116743224502591447233612598211, 0.20031873203795824700534922158113, 
    0.19901969626463703044727537871950, 0.20050097565137228738940677885694, 
    0.19787101498833312152036967290769, 0.19116743224502588671676051035320, 
    0.17104582243181359890726866979094, 0.19134568439589577648618501370947, 
    0.20021232807485411786529994060402, 0.19913131203074838082578423836821, 
    0.19259287894233698135160182118852, -0.17104582243181359890726866979094, 
    -0.19134568439589577648618501370947, -0.20021232807485411786529994060402, 
    -0.19913131203074838082578423836821, -0.19259287894233698135160182118852, 
    -0.17058670284035754027840425806062, -0.12129395515095907354119475485277, 
    -0.03607130478167273196499209575450, 0.07244269184042320675054327239195, 
    0.16336166612556776711606687513267, 0.17058670284035754027840425806062, 
    0.12129395515095930946358748769853, 0.03607130478167259318711401760993, 
    -0.07244269184042299858372615517510, -0.16336166612556776711606687513267, 
    0.19976310701392277491272864153871, 0.16323402056012617511449036555859, 
    0.06324205730973511685455434871983, -0.06051781170779292617911693241695, 
    -0.16052965288872450555501814051240, -0.19976310701392277491272864153871, 
    -0.16323402056012620287006598118751, -0.06324205730973513073234215653429, 
    0.06051781170779291230132912460249, 0.16052965288872450555501814051240
    };

// Old UGLY Declaration
/*
const double proj_r[80] = {
-0.00132029697684193014425857448657, -0.00058505123686783551927026447714, -0.00339468397069032836593782676005, -0.00040650650234961400140976106599, -0.00219078471377524104310441366295, 0.00132029697684190217184252436056, 0.00058505123686791336498624893636, 0.00339468397069037433610994014543, 0.00040650650234968924504053155999, 0.00219078471377530305946867983380, -0.10401748554092599130527219131181, -0.05837574642016117587584034254178, -0.02534602518731315576316909243815, -0.01173906302348584791694907636384, -0.00281456991227095219484932719922, 0.10401748554092603293863561475519, 0.05837574642016116893694643863455, 0.02534602518731318004929775611345, 0.01173906302348587740724816796956, 0.00281456991227097778202059785713, -0.10419838646349323385553731213804, -0.15737182038912753134773936380952, -0.19493752797443592017501146074210, -0.19436919790676290831221706412180, -0.15680021941424224229955086684640, 0.10419838646349324773332511995250, 0.15737182038912755910331497943844, 0.19493752797443592017501146074210, 0.19436919790676288055664144849288, 0.15680021941424218678839963558858, -0.00110369058882891146397553505665, 0.11406293921541321711998762111762, 0.18615930726484100632411866627081, 0.19716837111164342566382856603013, 0.15973801222277886346923025939759, 0.00110369058882893249749768127543, -0.11406293921541320324219981330316, -0.18615930726484103407969428189972, -0.19716837111164342566382856603013, -0.15973801222277864142462533436628, -0.00141366279319429452576706296441, -0.00285994240979238478761459418820, -0.01415919085618501035983562985621, -0.02615087290929190427801742657721, -0.05910515553292910312377372861192, 0.00141366279319428541846881408617, 0.00285994240979233101118683890718, 0.01415919085618514219881980409355, 0.02615087290929176550013934843264, 0.05910515553292924190165180675649, -0.10392385970452529264651531093477, -0.05622480885151811408340094544656, -0.01459459897167380768023825510227, 0.01403539744041460562984369175865, 0.05431356462776571608630504783832, 0.10392385970452532040209092656369, 0.05622480885151812102229484935378, 0.01459459897167383370109039475437, -0.01403539744041458481316198003697, -0.05431356462776569526962333611664, -0.10381139199392126759580179395925, -0.16067748314182234148361771985947, -0.19441140682922075644256665327703, -0.18827180589878331207209782860446, -0.11460877603955207937325155853614, 0.10381139199392130922916521740262, 0.16067748314182225821689087297273, 0.19441140682922078419814226890594, 0.18827180589878333982767344423337, 0.11460877603955207937325155853614, -0.00148861390352549114585167799873, 0.11790671198601079816548065082316, 0.18706804997001283408231131488719, 0.19257748293331786171300734622491, 0.11855691706085519365743152775394, 0.00148861390352552540664032854068, -0.11790671198601074265432941956533, -0.18706804997001277857116008362937, -0.19257748293331783395743173059600, -0.11855691706085520753521933556840
};
const double proj_i[80] = {
-0.20022994699815499508588345634053, -0.19930212538608207961665641505533, -0.20064714980915868136612800753937, -0.19983533762996610883000414560229, -0.19996481148887482182097130589682, 0.20022994699815499508588345634053, 0.19930212538608207961665641505533, 0.20064714980915868136612800753937, 0.19983533762996610883000414560229, 0.19996481148887482182097130589682, 0.17094374945445667268728584531345, 0.19087396185449426178060150505189, 0.19885754516415377057825253359624, 0.19952120033548864652672705233272, 0.19996601402686037252109940709488, -0.17094374945445667268728584531345, -0.19087396185449428953617712068080, -0.19885754516415371506710130233841, -0.19952120033548867428230266796163, -0.19996601402686037252109940709488, -0.17075199660761980480216948308225, -0.12321342678018226690639380649372, -0.04557004002377239987309209823252, 0.04660179542834700511244605536376, 0.12419528167265808660246761974122, 0.17075199660761977704659386745334, 0.12321342678018232241754503775155, 0.04557004002377246926203113730480, -0.04660179542834712307364242178664, -0.12419528167265811435804323537013, 0.19991334030230489826607254144619, 0.16462218757988547612747254333954, 0.07227703165261167850275114687975, -0.03420387753798159041762616539017, -0.12034449819846976259540838327666, -0.19991334030230492602164815707511, -0.16462218757988550388304815896845, -0.07227703165261160911381210780746, 0.03420387753798150715089931850343, 0.12034449819847002627337673175134, -0.20031873203795824700534922158113, -0.19901969626463703044727537871950, -0.20050097565137225963383116322802, -0.19787101498833303825364282602095, -0.19116743224502591447233612598211, 0.20031873203795824700534922158113, 0.19901969626463703044727537871950, 0.20050097565137228738940677885694, 0.19787101498833312152036967290769, 0.19116743224502588671676051035320, 0.17104582243181359890726866979094, 0.19134568439589577648618501370947, 0.20021232807485411786529994060402, 0.19913131203074838082578423836821, 0.19259287894233698135160182118852, -0.17104582243181359890726866979094, -0.19134568439589577648618501370947, -0.20021232807485411786529994060402, -0.19913131203074838082578423836821, -0.19259287894233698135160182118852, -0.17058670284035754027840425806062, -0.12129395515095907354119475485277, -0.03607130478167273196499209575450, 0.07244269184042320675054327239195, 0.16336166612556776711606687513267, 0.17058670284035754027840425806062, 0.12129395515095930946358748769853, 0.03607130478167259318711401760993, -0.07244269184042299858372615517510, -0.16336166612556776711606687513267, 0.19976310701392277491272864153871, 0.16323402056012617511449036555859, 0.06324205730973511685455434871983, -0.06051781170779292617911693241695, -0.16052965288872450555501814051240, -0.19976310701392277491272864153871, -0.16323402056012620287006598118751, -0.06324205730973513073234215653429, 0.06051781170779291230132912460249, 0.16052965288872450555501814051240
};
*/

    /* Translation of binary_symbols: */
    const int binary_symbols[2] = {
    0, 1
    };

    /* Translation of trellis: */
    const int trellis[32] = {
    0, 1, 1, 0, 2, 3, 3, 2, 4, 5, 5, 4, 6, 7, 7, 6, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    };

    /* Translation of anti_trellis: */
    const int anti_trellis[32] = {
    0, 3, 1, 2, 4, 7, 5, 6, 0, 3, 1, 2, 4, 7, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1
    };

private:
    //
    //  BCJR FUNCTIONS FOR DEMODULATION ////////////////////////////////////////////////////////////////////////////////
    //

    /* BCJR Algorithm */
    void BCJR_LLR2LogSymbProba( const mipp::vector<Q> &llr, 
                                      mipp::vector<Q> &log_proba_msg_symb); //const; // Changed To allow Coset Approach in IterDEV

    void BCJR_bits_probas(const mipp::vector<Q> &proba_msg_symb,            const B       *binary_symbols, 
                          const int             M,                          const int     cpm_bits_message_length, 
                          const int             cpm_symb_message_length,    const int     nb_bits_per_input_symb, 
                                mipp::vector<Q> &proba_msg_bits); //const; // Changed To allow Coset Approach in IterDEV

    void BCJR_symboles_probas(const B               *trellis,                   const mipp::vector<Q> &alpha, 
                              const mipp::vector<Q> &beta,                      const mipp::vector<Q> &gamma, 
                              const int             cpm_symb_message_length,    const int M,
                                    mipp::vector<Q> &proba_msg_symb); //const; // Changed To allow Coset Approach in IterDEV

    void BCJR_compute_alpha_beta_gamma( const B               *trellis,               const B   *anti_trellis, 
                                        const mipp::vector<Q> &symbol_apriori_prob,   const int M, 
                                        const int             cpm_symb_message_length, 
                                        const int             nb_output_symbs,        const Q   noise_var, 
                                        const mipp::vector<Q> &signal, 

                                              mipp::vector<Q> &gamma,                 mipp::vector<Q> &alpha, 
                                              mipp::vector<Q> &beta); //const; // Changed To allow Coset Approach in IterDEV

    void BCJR_compute_ext(  const mipp::vector<Q> &proba_msg_bits, const mipp::vector<Q> &llr,
                                  mipp::vector<Q> &L_ext); //const; // Changed To allow Coset Approach in IterDEV

};

#include "Modulator_GSM.hxx"

#endif /* MODULATOR_GSM_HPP_ */
