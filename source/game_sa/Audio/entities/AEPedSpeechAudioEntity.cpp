#include "StdInc.h"
#include <bitset>
#include "AEPedSpeechAudioEntity.h"
#include "AEAudioUtility.h"
#include "PedClothesDesc.h"
#include <AEAudioHardware.h>
#include <Audio/eSoundBankSlot.h>

// clang-format off

//
// Speech Info LUTs
//
namespace {
// TIP: Disable word-wrap when viewing this array... :D
// 0x8C6A68
static constexpr std::array<tGlobalSpeechContextInfo, CTX_GLOBAL_NUM> gSpeechContextLookup{{
    // GCtx                                     PED_TYPE_GEN                   PED_TYPE_EMG             PED_TYPE_PLAYER                      PED_TYPE_GANG                     PED_TYPE_GFD                               RepeatTime Zero           IsImportantFor...
    //                                                                                                                                                                                                                                    Streaming Interrupting Widescreen                                                       
    {CTX_GLOBAL_NO_SPEECH,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ABUSE_GANG_BALLAS,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ABUSE_GANG_BALLAS,        CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ABUSE_GANG_LSV,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ABUSE_GANG_LSV,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ABUSE_GANG_VLA,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ABUSE_GANG_VLA,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ABUSE_GANG_FAMILIES,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ABUSE_GANG_FAMILIES,      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ABUSE_TRIAD,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ABUSE_MAFIA,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ABUSE_RIFA,                     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ABUSE_DA_NANG,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ACCEPT_DATE_CALL,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ACCEPT_DATE_CALL,            CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ACCEPT_DATE_REQUEST,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ACCEPT_DATE_REQUEST,         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_AFTER_SEX,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_AFTER_SEX,                   CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_APOLOGY,                        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_APOLOGY,                     CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ARREST,                         {CTX_GEN_UNK,                  CTX_EMG_ARREST,          CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   true,     true,        true },
    {CTX_GLOBAL_ARREST_CRIM,                    {CTX_GEN_UNK,                  CTX_EMG_ARREST_CRIM,     CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ARRESTED,                       {CTX_GEN_ARRESTED,             CTX_EMG_UNK,             CTX_PLY_ARRESTED,                    CTX_GNG_ARRESTED,                 CTX_GFD_UNK,                            }, 7000,      0,   true,     true,        true },
    {CTX_GLOBAL_ATTACK_BY_PLAYER_LIKE,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ATTACK_BY_PLAYER_LIKE,    CTX_GFD_UNK,                            }, 5000,      0,   false,    false,       false},
    {CTX_GLOBAL_ATTACK_GANG_BALLAS,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ATTACK_GANG_BALLAS,       CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ATTACK_GANG_LSV,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ATTACK_GANG_LSV,          CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ATTACK_GANG_VLA,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ATTACK_GANG_VLA,          CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ATTACK_PLAYER,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ATTACK_PLAYER,            CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_BAR_CHAT,                       {CTX_GEN_BAR_CHAT,             CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_BAR_CHAT,                 CTX_GFD_UNK,                            }, 20000,     0,   false,    false,       false},
    {CTX_GLOBAL_BLOCKED,                        {CTX_GEN_BLOCKED,              CTX_EMG_BLOCKED,         CTX_PLY_UNK,                         CTX_GNG_BLOCKED,                  CTX_GFD_BLOCKED,                        }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_BOOZE_RECEIVE,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_BOOZE_RECEIVE,            CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_BOOZE_REQUEST,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_BOOZE_REQUEST,            CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_BUM_BACK_OFF,                   {CTX_GEN_BUM_BACK_OFF,         CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_BUM_BACK_OFF_2,                 {CTX_GEN_BUM_BACK_OFF_2,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_BUM_LATCH,                      {CTX_GEN_BUM_LATCH,            CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_BUMP,                           {CTX_GEN_BUMP,                 CTX_EMG_BUMP,            CTX_PLY_UNK,                         CTX_GNG_BUMP,                     CTX_GFD_BUMP,                           }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_CRASH,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_CRASH,                CTX_GFD_CAR_CRASH,                      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_DRIVEBY_BURN_RUBBER,        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_DRIVEBY_BURN_RUBBER,  CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_DRIVEBY_TOO_FAST,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_DRIVEBY_TOO_FAST,     CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_FAST,                       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_FAST,                 CTX_GFD_CAR_FAST,                       }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_FIRE,                       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_FIRE,                 CTX_GFD_CAR_FIRE,                       }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_FLIPPED,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_FLIPPED,              CTX_GFD_CAR_FLIPPED,                    }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_GET_IN,                     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_GET_IN,               CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_HIT_PED,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_HIT_PED,              CTX_GFD_CAR_HIT_PED,                    }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_JUMP,                       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_JUMP,                 CTX_GFD_CAR_JUMP,                       }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_POLICE_PURSUIT,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_POLICE_PURSUIT,       CTX_GFD_CAR_POLICE_PURSUIT,             }, 15000,     0,   false,    false,       false},
    {CTX_GLOBAL_CAR_SLOW,                       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_SLOW,                 CTX_GFD_CAR_SLOW,                       }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_WAIT_FOR_ME,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CAR_WAIT_FOR_ME,          CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CAR_SINGALONG,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CB_CHAT,                        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 20000,     0,   false,    false,       false},
    {CTX_GLOBAL_CHASE_FOOT,                     {CTX_GEN_UNK,                  CTX_EMG_CHASE_FOOT,      CTX_PLY_UNK,                         CTX_GNG_CHASE_FOOT,               CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CHASED,                         {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_CHASED,                      CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CHAT,                           {CTX_GEN_CHAT,                 CTX_EMG_CHAT,            CTX_PLY_UNK,                         CTX_GNG_CHAT,                     CTX_GFD_CHAT,                           }, 15000,     0,   false,    false,       false},
    {CTX_GLOBAL_COFFEE_ACCEPT,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_COFFEE_ACCEPT,               CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_COFFEE_DECLINE,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_COFFEE_DECLINE,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CONV_DISL_CAR,                  {CTX_GEN_CONV_DISL_CAR,        CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_DISL_CAR,            CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_DISL_CLOTHES,              {CTX_GEN_CONV_DISL_CLOTHES,    CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_DISL_CLOTHES,        CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_DISL_HAIR,                 {CTX_GEN_CONV_DISL_HAIR,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_DISL_HAIR,           CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_DISL_PHYS,                 {CTX_GEN_CONV_DISL_PHYS,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_DISL_PHYS,           CTX_GFD_CONV_DISL_PHYS,                 }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_DISL_SHOES,                {CTX_GEN_CONV_DISL_SHOES,      CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_DISL_SHOES,          CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_DISL_SMELL,                {CTX_GEN_CONV_DISL_SMELL,      CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_DISL_TATTOO,               {CTX_GEN_CONV_DISL_TATTOO,     CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_DISL_TATTOO,         CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_DISL_WEATHER,              {CTX_GEN_CONV_DISL_WEATHER,    CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_IGNORED,                   {CTX_GEN_CONV_IGNORED,         CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_IGNORED,             CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_LIKE_CAR,                  {CTX_GEN_CONV_LIKE_CAR,        CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_LIKE_CAR,            CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_LIKE_CLOTHES,              {CTX_GEN_CONV_LIKE_CLOTHES,    CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_LIKE_CLOTHES,        CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_LIKE_HAIR,                 {CTX_GEN_CONV_LIKE_HAIR,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_LIKE_HAIR,           CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_LIKE_PHYS,                 {CTX_GEN_CONV_LIKE_PHYS,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_LIKE_PHYS,           CTX_GFD_CONV_LIKE_PHYS,                 }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_LIKE_SHOES,                {CTX_GEN_CONV_LIKE_SHOES,      CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_LIKE_SHOES,          CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_LIKE_SMELL,                {CTX_GEN_CONV_LIKE_SMELL,      CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_LIKE_TATTOO,               {CTX_GEN_CONV_LIKE_TATTOO,     CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CONV_LIKE_TATTOO,         CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_CONV_LIKE_WEATHER,              {CTX_GEN_CONV_LIKE_WEATHER,    CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_COVER_ME,                       {CTX_GEN_UNK,                  CTX_EMG_COVER_ME,        CTX_PLY_UNK,                         CTX_GNG_COVER_ME,                 CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CRASH_BIKE,                     {CTX_GEN_CRASH_BIKE,           CTX_EMG_CRASH_BIKE,      CTX_PLY_UNK,                         CTX_GNG_CRASH_BIKE,               CTX_GFD_CRASH_BIKE,                     }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CRASH_CAR,                      {CTX_GEN_CRASH_CAR,            CTX_EMG_CRASH_CAR,       CTX_PLY_CRASH_CAR,                   CTX_GNG_CRASH_CAR,                CTX_GFD_CRASH_CAR,                      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CRASH_GENERIC,                  {CTX_GEN_CRASH_GENERIC,        CTX_EMG_CRASH_GENERIC,   CTX_PLY_CRASH_GENERIC,               CTX_GNG_CRASH_GENERIC,            CTX_GFD_CRASH_GENERIC,                  }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CRIMINAL_PLEAD,                 {CTX_GEN_CRIMINAL_PLEAD,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CRIMINAL_PLEAD,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DANCE_IMPR_HIGH,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DANCE_IMPR_LOW,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DANCE_IMPR_MED,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DANCE_IMPR_NOT,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DODGE,                          {CTX_GEN_DODGE,                CTX_EMG_DODGE,           CTX_PLY_UNK,                         CTX_GNG_DODGE,                    CTX_GFD_DODGE,                          }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUG_AGGRESSIVE_HIGH,           {CTX_GEN_DRUG_AGGRESSIVE_HIGH, CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUG_AGGRESSIVE_LOW,            {CTX_GEN_DRUG_AGGRESSIVE_LOW,  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUG_DEALER_DISLIKE,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_DRUG_DEALER_DISLIKE,         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUG_DEALER_HATE,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_DRUG_DEALER_HATE,            CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUG_REASONABLE_HIGH,           {CTX_GEN_DRUG_REASONABLE_HIGH, CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUG_REASONABLE_LOW,            {CTX_GEN_DRUG_REASONABLE_LOW,  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUGGED_CHAT,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_DRUGGED_CHAT,             CTX_GFD_UNK,                            }, 20000,     0,   false,    false,       false},
    {CTX_GLOBAL_DRUGGED_IGNORE,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_DRUGGED_IGNORE,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUGS_BUY,                      {CTX_GEN_DRUGS_BUY,            CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_DRUGS_BUY,                CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRUGS_SELL,                     {CTX_GEN_DRUGS_SELL,           CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DUCK,                           {CTX_GEN_UNK,                  CTX_EMG_DUCK,            CTX_PLY_UNK,                         CTX_GNG_DUCK,                     CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_EYEING_PED,                     {CTX_GEN_EYEING_PED,           CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_EYEING_PED,               CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_EYEING_PED_THREAT,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_EYEING_PED_THREAT,        CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_EYEING_PLAYER,                  {CTX_GEN_EYEING_PLAYER,        CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_EYEING_PLAYER,            CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_FIGHT,                          {CTX_GEN_FIGHT,                CTX_EMG_FIGHT,           CTX_PLY_FIGHT,                       CTX_GNG_FIGHT,                    CTX_GFD_FIGHT,                          }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_FOLLOW_ARRIVE,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_FOLLOW_ARRIVE,            CTX_GFD_UNK,                            }, 15000,     0,   false,    false,       false},
    {CTX_GLOBAL_FOLLOW_CONSTANT,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_FOLLOW_CONSTANT,          CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_FOLLOW_REPLY,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_FOLLOW_REPLY,             CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_BJ_HIT,                    {CTX_GEN_GAMB_BJ_HIT,          CTX_EMG_UNK,             CTX_PLY_BJ_HIT,                      CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_BJ_LOSE,                   {CTX_GEN_GAMB_BJ_LOSE,         CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_BJ_STAY,                   {CTX_GEN_GAMB_BJ_STAY,         CTX_EMG_UNK,             CTX_PLY_BJ_STICK,                    CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_BJ_WIN,                    {CTX_GEN_GAMB_BJ_WIN,          CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_BJ_STICK,                  {CTX_GEN_GAMB_BJ_STAY,         CTX_EMG_UNK,             CTX_PLY_BJ_STICK,                    CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_BJ_DOUBLE,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_BJ_DOUBLE,                   CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_BJ_SPLIT,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_BJ_SPLIT,                    CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_CASINO_WIN,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_CASINO_WIN,                  CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_CASINO_LOSE,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_CASINO_LOSE,                 CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_CONGRATS,                  {CTX_GEN_GAMB_CONGRATS,        CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_ROUL_CHAT,                 {CTX_GEN_GAMB_ROUL_CHAT,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 20000,     0,   false,    false,       false},
    {CTX_GLOBAL_GAMB_SLOT_WIN,                  {CTX_GEN_GAMB_SLOT_WIN,        CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GANG_FULL,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_GANG_FULL,                CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GANGBANG_NO,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GANGBANG_NO,                 CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GANGBANG_YES,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GANGBANG_YES,                CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GENERIC_HI_MALE,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GENERIC_HI_MALE,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GENERIC_HI_FEMALE,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GENERIC_HI_FEMALE,           CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GENERIC_INSULT_MALE,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GENERIC_INSULT_MALE,         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GENERIC_INSULT_FEMALE,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GENERIC_INSULT_FEMALE,       CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GIVING_HEAD,                    {CTX_GEN_GIVING_HEAD,          CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GOOD_CITIZEN,                   {CTX_GEN_UNK,                  CTX_EMG_GOOD_CITIZEN,    CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GUN_COOL,                       {CTX_GEN_GUN_COOL,             CTX_EMG_GUN_COOL,        CTX_PLY_UNK,                         CTX_GNG_GUN_COOL,                 CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GUN_RUN,                        {CTX_GEN_GUN_RUN,              CTX_EMG_GUN_RUN,         CTX_PLY_UNK,                         CTX_GNG_GUN_RUN,                  CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_HAVING_SEX,                     {CTX_GEN_HAVING_SEX,           CTX_EMG_UNK,             CTX_PLY_HAVING_SEX,                  CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 2500,      0,   false,    false,       true },
    {CTX_GLOBAL_HAVING_SEX_MUFFLED,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_HAVING_SEX_MUFFLED,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 2500,      0,   false,    false,       false},
    {CTX_GLOBAL_JACKED_GENERIC,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JACKED_GENERIC,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_JACKED_CAR,                     {CTX_GEN_JACKED_IN_CAR,        CTX_EMG_UNK,             CTX_PLY_JACKED_CAR,                  CTX_GNG_JACKED_IN_CAR,            CTX_GFD_JACKED_IN_CAR,                  }, 1000,      0,   false,    false,       false},
    {CTX_GLOBAL_JACKED_ON_STREET,               {CTX_GEN_JACKED_ON_STREET,     CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_JACKED_ON_STREET,         CTX_GFD_JACKED_ON_STREET,               }, 1000,      0,   false,    false,       false},
    {CTX_GLOBAL_JACKING_BIKE,                   {CTX_GEN_JACKING_GENERIC,      CTX_EMG_JACKING_GENERIC, CTX_PLY_JACKING_BIKE,                CTX_GNG_JACKING_GENERIC,          CTX_GFD_JACKING_GENERIC,                }, 1000,      0,   false,    false,       false},
    {CTX_GLOBAL_JACKING_CAR_FEM,                {CTX_GEN_JACKING_CAR,          CTX_EMG_JACKING_GENERIC, CTX_PLY_JACKING_CAR_FEM,             CTX_GNG_JACKING_CAR,              CTX_GFD_JACKING_GENERIC,                }, 1000,      0,   false,    false,       false},
    {CTX_GLOBAL_JACKING_CAR_MALE,               {CTX_GEN_JACKING_CAR,          CTX_EMG_JACKING_GENERIC, CTX_PLY_JACKING_CAR_MALE,            CTX_GNG_JACKING_CAR,              CTX_GFD_JACKING_GENERIC,                }, 1000,      0,   false,    false,       false},
    {CTX_GLOBAL_JACKING_GENERIC,                {CTX_GEN_JACKING_GENERIC,      CTX_EMG_JACKING_GENERIC, CTX_PLY_JACKING_GENERIC,             CTX_GNG_JACKING_GENERIC,          CTX_GFD_JACKING_GENERIC,                }, 1000,      0,   false,    false,       false},
    {CTX_GLOBAL_JOIN_GANG_NO,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_JOIN_GANG_NO,             CTX_GFD_UNK,                            }, 1500,      0,   true,     true,        false},
    {CTX_GLOBAL_JOIN_GANG_YES,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_JOIN_GANG_YES,            CTX_GFD_UNK,                            }, 1500,      0,   true,     true,        false},
    {CTX_GLOBAL_JOIN_ME_ASK,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JOIN_ME_ASK,                 CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 1000,      0,   true,     true,        false},
    {CTX_GLOBAL_JOIN_ME_REJECTED,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JOIN_ME_REJECTED,            CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_CAR_REPLY,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_CAR_REPLY,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_CLOTHES_REPLY,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_CLOTHES_REPLY,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_DISMISS_FEMALE,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_DISMISS_FEMALE,         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_DISMISS_MALE,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_DISMISS_MALE,           CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_DISMISS_REPLY,             {CTX_GEN_LIKE_DISMISS_REPLY,   CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_LIKE_DISMISS_REPLY,       CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_HAIR_REPLY,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_HAIR_REPLY,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_NEGATIVE_FEMALE,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_NEGATIVE_FEMALE,        CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_NEGATIVE_MALE,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_NEGATIVE_MALE,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_PHYS_REPLY,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_PHYS_REPLY,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_SHOES_REPLY,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_SHOES_REPLY,            CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_SMELL_REPLY,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_SMELL_REPLY,            CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_LIKE_TATTOO_REPLY,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_LIKE_TATTOO_REPLY,           CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_MEET_GFRIEND_AGAIN_MAYBE,       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_MEET_GFRIEND_AGAIN_MAYBE,    CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_MEET_GFRIEND_AGAIN_NO,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_MEET_GFRIEND_AGAIN_NO,       CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_MEET_GFRIEND_AGAIN_YES,         {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_MEET_GFRIEND_AGAIN_YES,      CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_MOVE_IN,                        {CTX_GEN_UNK,                  CTX_EMG_MOVE_IN,         CTX_PLY_UNK,                         CTX_GNG_MOVE_IN,                  CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_MUGGED,                         {CTX_GEN_MUGGED,               CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_MUGGED,                   CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_MUGGING,                        {CTX_GEN_MUGGING,              CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_MUGGING,                  CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_ATTACK_MANY,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_ATTACK_MANY,           CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_ATTACK_SINGLE,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_ATTACK_SINGLE,         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_DISBAND_MANY,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_DISBAND_MANY,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_DISBAND_ONE,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_DISBAND_ONE,           CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_FOLLOW_FAR_MANY,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_FOLLOW_FAR_MANY,       CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_FOLLOW_FAR_ONE,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_FOLLOW_FAR_ONE,        CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_FOLLOW_NEAR_MANY,         {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_FOLLOW_NEAR_MANY,      CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_FOLLOW_NEAR_ONE,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_FOLLOW_NEAR_ONE,       CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_FOLLOW_VNEAR_MANY,        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_FOLLOW_VNEAR_MANY,     CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_FOLLOW_VNEAR_ONE,         {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_FOLLOW_VNEAR_ONE,      CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_KEEP_UP_MANY,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_KEEP_UP_MANY,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_KEEP_UP_ONE,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_KEEP_UP_ONE,           CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_WAIT_MANY,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_WAIT_MANY,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ORDER_WAIT_ONE,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ORDER_WAIT_ONE,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_PCONV_AGREE_BAD,                {CTX_GEN_PCONV_AGREE_BAD,      CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_AGREE_BAD,          CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_AGREE_GOOD,               {CTX_GEN_PCONV_AGREE_GOOD,     CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_AGREE_GOOD,         CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_ANS_NO,                   {CTX_GEN_PCONV_ANS_NO,         CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_ANS_NO,             CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_DISMISS,                  {CTX_GEN_PCONV_DISMISS,        CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_DISMISS,            CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_GREET_FEM,                {CTX_GEN_PCONV_GREET_FEM,      CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_GREET_FEM,          CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_GREET_MALE,               {CTX_GEN_PCONV_GREET_MALE,     CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_GREET_MALE,         CTX_GFD_UNK,                            }, 3000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_PART_FEM,                 {CTX_GEN_PCONV_PART_FEM,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_PART_FEM,           CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_PART_MALE,                {CTX_GEN_PCONV_PART_MALE,      CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_PART_MALE,          CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_QUESTION,                 {CTX_GEN_PCONV_QUESTION,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_QUESTION,           CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_STATE_BAD,                {CTX_GEN_PCONV_STATE_BAD,      CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_STATE_BAD,          CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PCONV_STATE_GOOD,               {CTX_GEN_PCONV_STATE_GOOD,     CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PCONV_STATE_GOOD,         CTX_GFD_UNK,                            }, 7000,      0,   true,     false,       false},
    {CTX_GLOBAL_PICKUP_CASH,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_PICKUP_CASH,                 CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_POLICE_BOAT,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_BOAT,                           }, 10000,     0,   false,    false,       false},
    {CTX_GLOBAL_POLICE_HELICOPTER,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_HELICOPTER,                     }, 10000,     0,   false,    false,       false},
    {CTX_GLOBAL_POLICE_OVERBOARD,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_OVERBOARD,                      }, 10000,     0,   false,    false,       false},
    {CTX_GLOBAL_PULL_GUN,                       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_PULL_GUN,                    CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 20000,     0,   false,    false,       false},
    {CTX_GLOBAL_ROPE,                           {CTX_GEN_UNK,                  CTX_EMG_ROPE,            CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_RUN_FROM_FIGHT,                 {CTX_GEN_RUN_FROM_FIGHT,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_RUN_FROM_FIGHT,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SAVED,                          {CTX_GEN_SAVED,                CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_SAVED,                    CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SEARCH,                         {CTX_GEN_UNK,                  CTX_EMG_SEARCH,          CTX_PLY_UNK,                         CTX_GNG_SEARCH,                   CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOCKED,                        {CTX_GEN_SHOCKED,              CTX_EMG_SHOCKED,         CTX_PLY_UNK,                         CTX_GNG_SHOCKED,                  CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOOT,                          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SHOOT,                       CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOOT_BALLAS,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_SHOOT_BALLAS,             CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOOT_GENERIC,                  {CTX_GEN_UNK,                  CTX_EMG_SHOOT_GENERIC,   CTX_PLY_UNK,                         CTX_GNG_SHOOT_GENERIC,            CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOOT_LSV,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_SHOOT_LSV,                CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOOT_VLA,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_SHOOT_VLA,                CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOOT_FAMILIES,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_SHOOT_FAMILIES,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOP_BROWSE,                    {CTX_GEN_SHOP_BROWSE,          CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOP_BUY,                       {CTX_GEN_SHOP_BUY,             CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOP_SELL,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GIVE_PRODUCT,                   }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOP_LEAVE,                     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_THANKS_FOR_CUSTOM,              }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SOLICIT,                        {CTX_GEN_SOLICIT,              CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SOLICIT_GEN_NO,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SOLICIT_GEN_NO,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SOLICIT_GEN_YES,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SOLICIT_GEN_YES,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SOLICIT_PRO_NO,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SOLICIT_PRO_NO,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SOLICIT_PRO_YES,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SOLICIT_PRO_YES,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SOLICIT_THANKS,                 {CTX_GEN_SOLICIT_THANKS,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SOLICIT_UNREASONABLE,           {CTX_GEN_SOLICIT_UNREASONABLE, CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SOLO,                           {CTX_GEN_UNK,                  CTX_EMG_SOLO,            CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SPLIFF_RECEIVE,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_SPLIFF_RECEIVE,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SPLIFF_REQUEST,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_SPLIFF_REQUEST,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_STEALTH_ALERT_SOUND,            {CTX_GEN_ALERT_SOUND,          CTX_EMG_ALERT_SOUND,     CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_STEALTH_ALERT_SIGHT,            {CTX_GEN_ALERT_SIGHT,          CTX_EMG_ALERT_SIGHT,     CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_STEALTH_ALERT_GENERIC,          {CTX_GEN_ALERT_GENERIC,        CTX_EMG_ALERT_GENERIC,   CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_STEALTH_DEF_SIGHTING,           {CTX_GEN_DEF_SIGHTING,         CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_STEALTH_NOTHING_THERE,          {CTX_GEN_UNK,                  CTX_EMG_NOTHING_THERE,   CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SURROUNDED,                     {CTX_GEN_UNK,                  CTX_EMG_SURROUNDED,      CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TARGET,                         {CTX_GEN_UNK,                  CTX_EMG_TARGET,          CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_LAS_COLINAS,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_LOS_FLORES,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_EAST_BEACH,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_EAST_LS,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_JEFFERSON,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_GLEN_PARK,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_IDLEWOOD,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_GANTON,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_LITTLE_MEXICO,        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_WILLOWFIELD,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_PLAYA_DEL_SEVILLE,    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_TURF_TEMPLE,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAXI_BAIL,                      {CTX_GEN_TAXI_BAIL,            CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAXI_HAIL,                      {CTX_GEN_TAXI_HAIL,            CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_TAXI_HAIL,                CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAXI_HIT_PED,                   {CTX_GEN_TAXI_HIT_PED,         CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAXI_START,                     {CTX_GEN_TAXI_START,           CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAXI_SUCCESS,                   {CTX_GEN_TAXI_SUCCESS,         CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TAXI_TIP,                       {CTX_GEN_TAXI_TIP,             CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TRAPPED,                        {CTX_GEN_TRAPPED,              CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_TRAPPED,                  CTX_GFD_UNK,                            }, 3500,      0,   false,    false,       false},
    {CTX_GLOBAL_VALET_BAD,                      {CTX_GEN_VALET_BAD,            CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_VALET_GOOD,                     {CTX_GEN_VALET_GOOD,           CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_VALET_PARK_CAR,                 {CTX_GEN_VALET_PARK_CAR,       CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_VAN,                            {CTX_GEN_UNK,                  CTX_EMG_VAN,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_VICTIM,                         {CTX_GEN_UNK,                  CTX_EMG_VICTIM,          CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_WEATHER_DISL_REPLY,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_WEATHER_DISL_REPLY,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_WEATHER_LIKE_REPLY,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_WEATHER_LIKE_REPLY,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_WHERE_YOU_FROM_NEG,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_WHERE_YOU_FROM_NEG,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_WHERE_YOU_FROM_POS,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_WHERE_YOU_FROM_POS,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SPANKED,                        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SPANKED,                     CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SPANKING,                       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SPANKING,                    CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SPANKING_MUFFLED,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SPANKING_MUFFLED,            CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GREETING_GFRIEND,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GREETING_GFRIEND,            CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_PARTING_GFRIEND,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_PARTING_GFRIEND,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_UH_HUH,                         {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UH_HUH,                      CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CLEAR_ATTACHED_PEDS,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_CLEAR_ATTACHED_PEDS,         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_AGREE_TO_DO_DRIVEBY,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_AGREE_TO_DO_DRIVEBY,         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_AGREE_TO_LET_DRIVE,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_AGREE_TO_LET_DRIVE,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_MICHELLE_TAKE_CAR,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_MICHELLE_TAKE_CAR,           CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ACCEPT_SEX,                     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_ACCEPT_SEX,                  CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DECLINE_SEX,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_DECLINE_SEX,                 CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GIVE_NUMBER_YES,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GIVE_NUMBER_YES,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GIVE_NUMBER_NO,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_GIVE_NUMBER_NO,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_WHERE_YOU_FROM,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_WHERE_YOU_FROM,           CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GANGBANG,                       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_GANGBANG,                 CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_WHERE_YOU_FROM_POS_REPLY,       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_WHERE_YOU_FROM_POS_REPLY, CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_DRIVE_THROUGH_TAUNT,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_DRIVE_THROUGH_TAUNT,      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ATTACK_CAR,                     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ATTACK_CAR,               CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_TIP_CAR,                        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_TIP_CAR,                  CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CHASE_CAR,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_CHASE_CAR,                CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_ENEMY_GANG_WASTED,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_ENEMY_GANG_WASTED,        CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_PLAYER_WASTED,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_PLAYER_WASTED,            CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_CHASE,                          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_DATE_DESPERATE,     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_DATE_DESPERATE,     }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_DATE_NORMAL,        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_DATE_NORMAL,        }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_MEAL_DESPERATE,     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_MEAL_DESPERATE,     }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_MEAL_NORMAL,        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_MEAL_NORMAL,        }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_DRIVE_DESPERATE,    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_DRIVE_DESPERATE,    }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_DRIVE_NORMAL,       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_DRIVE_NORMAL,       }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_DANCE_DESPERATE,    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_DANCE_DESPERATE,    }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_DANCE_NORMAL,       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_DANCE_NORMAL,       }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_SEX_DESPERATE,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_SEX_DESPERATE,      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQ_SEX_NORMAL,         {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQ_SEX_NORMAL,         }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_BORED_1,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_BORED_1,                }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_BORED_2,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_BORED_2,                }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_STORY,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_STORY,                  }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_LIKE_MEAL_DEST,         {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_LIKE_MEAL_DEST,         }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_LIKE_CLUB_DEST,         {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_LIKE_CLUB_DEST,         }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_OFFER_DANCE,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_OFFER_DANCE,            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_ENJOYED_MEAL_HIGH,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_ENJOYED_MEAL_HIGH,      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_ENJOYED_EVENT_LOW,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_ENJOYED_EVENT_LOW,      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_ENJOYED_CLUB_HIGH,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_ENJOYED_CLUB_HIGH,      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_TAKE_HOME_HAPPY,        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_TAKE_HOME_HAPPY,        }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_TAKE_HOME_ANGRY,        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_TAKE_HOME_ANGRY,        }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_COFFEE,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_COFFEE,                 }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_MOAN,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_MOAN,                   }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_MOAN_MUFFLED,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_MOAN_MUFFLED,           }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_HEAD,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_HEAD,                   }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_CLIMAX_HIGH,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_CLIMAX_HIGH,            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_CLIMAX_HIGH_MUFFLED,    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_CLIMAX_HIGH_MUFFLED,    }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_CLIMAX_LOW,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_CLIMAX_LOW,             }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_SEX_GOOD,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_SEX_GOOD,               }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_SEX_GOOD_MUFFLED,       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_SEX_GOOD_MUFFLED,       }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_SEX_BAD,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_SEX_BAD,                }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_MEET_AGAIN,             {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_MEET_AGAIN,             }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_JEALOUS,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_JEALOUS,                }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_JEALOUS_REPLY,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_JEALOUS_REPLY,          }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_GOODBYE_HAPPY,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_GOODBYE_HAPPY,          }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_GOODBYE_ANGRY,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_GOODBYE_ANGRY,          }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_LEFT_BEHIND,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_LEFT_BEHIND,            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_HELLO,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_HELLO,                  }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_GOODBYE,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_GOODBYE,                }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_PICKUP_LOCATION,        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_PARK_UP,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_PARK_UP,                }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_PARK_LOCATION_HATE,     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_PARK_LOCATION_HATE,     }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_GIFT_LIKE,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_GIFT_LIKE,              }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_CHANGE_RADIO_FAVE,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_CHANGE_RADIO_FAVE,      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_CHANGE_RADIO_BACK,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_CHANGE_RADIO_BACK,      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_DO_A_DRIVEBY,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_DO_A_DRIVEBY,           }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_START_A_FIGHT,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_START_A_FIGHT,          }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REJECT_DATE,            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REJECT_DATE,            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_REQUEST_TO_DRIVE_CAR,   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_REQUEST_TO_DRIVE_CAR,   }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_DROP_PLAYER_DRIVE_AWAY, {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_DROP_PLAYER_DRIVE_AWAY, }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_DISLIKE_CURRENT_ZONE,   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_DISLIKE_CURRENT_ZONE,   }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_LIKE_CURRENT_ZONE,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_LIKE_CURRENT_ZONE,      }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_HIT_BY_PLAYER_WARNING,  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_HIT_BY_PLAYER_WARNING,  }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_DUMP_PLAYER_LIVE,       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_DUMP_PLAYER_LIVE,       }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_DUMP_PLAYER_PHONE,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_SEX_APPEAL_TOO_LOW,     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_SEX_APPEAL_TOO_LOW,             }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_PHYSIQUE_CRITIQUE,      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_PHYSIQUE_CRITIQUE,              }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_INTRO,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_INTRO,                  }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_NEG_RESPONSE,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_NEG_RESPONSE,           }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_GFRIEND_POS_RESPONSE,           {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GFRIEND_POS_RESPONSE,           }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_BOXING_CHEER,                   {CTX_GEN_BOXING_CHEER,         CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_BOUGHT_ENOUGH,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_BOUGHT_ENOUGH,                  }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_GIVE_PRODUCT,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_GIVE_PRODUCT,                   }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_NO_MONEY,                       {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_NO_MONEY,                       }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_PLAYER_SICK,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_PLAYER_SICK,                    }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_REMOVE_TATTOO,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_REMOVE_TATTOO,                  }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOP_CLOSED,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_SHOP_CLOSED,                    }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOW_CHANGINGROOM,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_SHOW_CHANGINGROOM,              }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_SHOP_CHAT,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_SHOP_CHAT,                      }, 15000,     0,   false,    false,       false},
    {CTX_GLOBAL_TAKE_A_SEAT,                    {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_TAKE_A_SEAT,                    }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_THANKS_FOR_CUSTOM,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_THANKS_FOR_CUSTOM,              }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_WELCOME_TO_SHOP,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_WELCOME_TO_SHOP,                }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_WHAT_WANT,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_WHAT_WANT,                      }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_PHOTO_CARL,                     {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_PHOTO_CARL,                  CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 4000,      0,   false,    false,       false},
    {CTX_GLOBAL_PHOTO_CHEESE,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_PHOTO_CHEESE,                CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 4000,      0,   false,    false,       false},
    {CTX_GLOBAL_SINGING,                        {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_SINGING,                     CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 30000,     0,   false,    false,       false},
    {CTX_GLOBAL_STOMACH_RUMBLE,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_STOMACH_RUMBLE,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 5000,      0,   false,    false,       false},
    {CTX_GLOBAL_BREATHING,                      {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_BREATHING,                   CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 4000,      0,   false,    false,       false},
    // Skipped CTX_GLOBAL_PAIN_START
    {CTX_GLOBAL_PAIN_COUGH,                     {CTX_GEN_LIKE_DISMISS_REPLY,   CTX_EMG_CRASH_GENERIC,   CTX_PLY_APOLOGY,                     CTX_GNG_CRIMINAL_PLEAD,           CTX_GFD_GFRIEND_CHANGE_RADIO_FAVE,      }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_DEATH_DROWN,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_DEATH_HIGH,                {CTX_GEN_MUGGED,               CTX_EMG_GUN_RUN,         CTX_PLY_ARRESTED,                    CTX_GNG_DODGE,                    CTX_GFD_GFRIEND_CLIMAX_HIGH,            }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_DEATH_LOW,                 {CTX_GEN_NOTHING_THERE,        CTX_EMG_VAN,             CTX_PLY_ARRESTED,                    CTX_GNG_DRIVE_THROUGH_TAUNT,      CTX_GFD_GFRIEND_CLIMAX_HIGH_MUFFLED,    }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_HIGH,                      {CTX_GEN_SAVED,                CTX_EMG_VICTIM,          CTX_PLY_FIGHT,                       CTX_GNG_DUCK,                     CTX_GFD_GFRIEND_CLIMAX_LOW,             }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_LOW,                       {CTX_GEN_SAVED,                CTX_EMG_VICTIM,          CTX_PLY_JACKED_CAR,                  CTX_GNG_DUCK,                     CTX_GFD_GFRIEND_CLIMAX_LOW,             }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_ON_FIRE,                   {CTX_GEN_SHOCKED,              CTX_EMG_SURROUNDED,      CTX_PLY_DRUG_DEALER_DISLIKE,         CTX_GNG_ENEMY_GANG_WASTED,        CTX_GFD_GFRIEND_COFFEE,                 }, 100,       0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_PANIC,                     {CTX_GEN_TRAPPED,              CTX_EMG_TARGET,          CTX_PLY_UNK,                         CTX_GNG_EYEING_PED,               CTX_GFD_GFRIEND_DISLIKE_CURRENT_ZONE,   }, 8000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_SPRAYED,                   {CTX_GEN_BAR_CHAT,             CTX_EMG_SEARCH,          CTX_PLY_UNK,                         CTX_GNG_EYEING_PED_THREAT,        CTX_GFD_GFRIEND_DUMP_PLAYER_LIVE,       }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_BOXING,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_AFTER_SEX,                   CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_GRUNT,                  {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_CRASH_CAR,                   CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_PUKE,                   {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JACKING_CAR_FEMALE,          CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_PANT_IN,                {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JACKED_GENERIC,              CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 1000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_PANT_OUT,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JACKING_BIKE,                CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 1000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_STRAIN,                 {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JACKING_CAR_MALE,            CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_STRAIN_EXHALE,          {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JACKING_GENERIC,             CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_SWIM_GASP,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_JOIN_ME_ASK,                 CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 10000,     0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_DROWNING,               {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_CHASED,                      CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 2000,      0,   false,    false,       false},
    {CTX_GLOBAL_PAIN_CJ_HIGH_FALL,              {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_CRASH_GENERIC,               CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 3000,      0,   false,    false,       false},
    // Skipped CTX_GLOBAL_PAIN_END
    {CTX_GLOBAL_END,                            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, 7000,      0,   false,    false,       false},
    {CTX_GLOBAL_UNK,                            {CTX_GEN_UNK,                  CTX_EMG_UNK,             CTX_PLY_UNK,                         CTX_GNG_UNK,                      CTX_GFD_UNK,                            }, -1,        0,   false,    false,       false},
}};

// 0x8AF8A8
static constexpr notsa::mdarray<tSpecificSpeechContextInfo, CTX_GEN_END, VOICE_GEN_END> gGenSpeechLookup{{
    {{{0, 2}, {0, 2}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 2}, {}, {}, {}, {}, {}}},
    {{{3, 4}, {3, 4}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {3, 4}, {}, {}, {}, {}, {}}},
    {{{5, 6}, {5, 6}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {5, 6}, {}, {}, {}, {}, {}}},
    {{{7, 10}, {7, 10}, {5, 10}, {6, 13}, {}, {3, 9}, {}, {}, {0, 7}, {6, 15}, {2, 9}, {2, 9}, {0, 5}, {6, 12}, {5, 11}, {0, 5}, {5, 13}, {}, {}, {}, {0, 3}, {}, {0, 9}, {}, {}, {11, 20}, {3, 12}, {}, {0, 5}, {}, {}, {5, 14}, {6, 12}, {}, {}, {4, 7}, {0, 5}, {0, 4}, {0, 5}, {7, 12}, {4, 8}, {6, 11}, {0, 7}, {6, 11}, {5, 14}, {}, {}, {0, 5}, {4, 8}, {5, 10}, {0, 4}, {6, 10}, {}, {0, 4}, {4, 10}, {4, 9}, {0, 4}, {5, 11}, {4, 10}, {6, 16}, {}, {}, {0, 9}, {4, 13}, {7, 16}, {3, 12}, {}, {}, {13, 23}, {}, {0, 7}, {0, 9}, {5, 12}, {0, 9}, {5, 14}, {}, {}, {4, 9}, {}, {2, 6}, {5, 9}, {3, 10}, {6, 14}, {}, {}, {6, 13}, {0, 6}, {8, 17}, {0, 9}, {7, 13}, {4, 7}, {}, {4, 8}, {10, 19}, {}, {4, 13}, {0, 9}, {3, 11}, {}, {}, {6, 8}, {6, 12}, {3, 12}, {}, {}, {}, {11, 19}, {6, 14}, {0, 8}, {5, 11}, {6, 11}, {5, 10}, {6, 13}, {4, 6}, {4, 11}, {5, 11}, {6, 12}, {}, {4, 9}, {4, 9}, {5, 11}, {}, {3, 8}, {4, 12}, {0, 5}, {0, 8}, {}, {3, 12}, {5, 14}, {0, 12}, {}, {}, {}, {}, {}, {8, 16}, {6, 13}, {5, 11}, {6, 15}, {7, 16}, {}, {}, {4, 10}, {11, 20}, {6, 12}, {}, {}, {9, 19}, {5, 13}, {6, 15}, {}, {}, {}, {}, {}, {}, {}, {0, 6}, {8, 16}, {}, {6, 13}, {5, 9}, {}, {6, 13}, {}, {}, {}, {}, {}, {0, 5}, {}, {0, 8}, {}, {}, {}, {}, {0, 4}, {}, {0, 7}, {}, {}, {0, 4}, {0, 3}, {}, {}, {5, 14}, {5, 14}, {8, 16}, {0, 5}, {0, 9}, {0, 6}, {7, 13}, {}, {0, 3}, {0, 2}, {0, 1}, {}, {}, {}, {0, 5}, {}, {0, 6}, {}, {7, 10}, {}, {0, 9}, {5, 12}, {9, 14}, {0, 3}}},
    {{{11, 15}, {11, 15}, {11, 21}, {14, 22}, {0, 4}, {10, 20}, {0, 4}, {4, 15}, {8, 17}, {16, 28}, {10, 17}, {10, 17}, {6, 13}, {13, 16}, {12, 23}, {}, {14, 25}, {26, 38}, {8, 20}, {0, 8}, {4, 8}, {10, 14}, {10, 20}, {0, 4}, {0, 7}, {21, 33}, {13, 22}, {13, 26}, {6, 10}, {0, 4}, {0, 4}, {15, 28}, {13, 23}, {8, 16}, {0, 14}, {8, 15}, {6, 20}, {5, 13}, {6, 14}, {13, 23}, {9, 14}, {12, 24}, {8, 19}, {12, 17}, {15, 25}, {0, 11}, {0, 8}, {6, 13}, {9, 18}, {11, 23}, {5, 15}, {11, 20}, {0, 12}, {5, 12}, {11, 22}, {10, 20}, {5, 10}, {12, 23}, {11, 25}, {17, 24}, {0, 10}, {0, 7}, {10, 19}, {14, 23}, {17, 30}, {13, 23}, {0, 10}, {0, 4}, {24, 36}, {16, 26}, {8, 23}, {10, 21}, {13, 27}, {10, 21}, {15, 24}, {}, {}, {10, 15}, {0, 0}, {7, 10}, {10, 17}, {11, 20}, {15, 31}, {}, {0, 4}, {14, 27}, {7, 16}, {18, 32}, {10, 21}, {14, 23}, {8, 17}, {6, 19}, {9, 14}, {20, 32}, {}, {14, 16}, {10, 20}, {12, 17}, {28, 40}, {21, 28}, {9, 15}, {13, 20}, {13, 22}, {24, 35}, {7, 13}, {11, 20}, {20, 31}, {15, 27}, {9, 20}, {12, 20}, {12, 21}, {11, 19}, {14, 19}, {7, 10}, {12, 17}, {12, 23}, {13, 20}, {6, 12}, {10, 16}, {10, 14}, {12, 21}, {5, 22}, {9, 16}, {13, 22}, {6, 18}, {9, 16}, {}, {13, 15}, {15, 24}, {13, 24}, {20, 33}, {16, 25}, {21, 35}, {24, 34}, {28, 38}, {17, 29}, {14, 22}, {12, 24}, {16, 25}, {17, 28}, {0, 11}, {}, {11, 15}, {21, 30}, {13, 20}, {8, 20}, {}, {20, 29}, {14, 23}, {16, 27}, {6, 18}, {}, {0, 4}, {23, 34}, {32, 40}, {4, 12}, {0, 4}, {7, 12}, {17, 25}, {}, {14, 22}, {10, 15}, {0, 11}, {14, 24}, {0, 4}, {0, 4}, {0, 13}, {0, 13}, {7, 21}, {6, 18}, {7, 16}, {9, 16}, {0, 4}, {0, 4}, {}, {}, {5, 13}, {0, 4}, {8, 18}, {28, 36}, {0, 9}, {5, 9}, {}, {10, 14}, {0, 7}, {15, 26}, {15, 22}, {17, 22}, {6, 10}, {10, 21}, {7, 18}, {14, 23}, {5, 11}, {4, 8}, {3, 7}, {2, 6}, {0, 9}, {0, 6}, {0, 4}, {6, 10}, {0, 4}, {7, 14}, {0, 11}, {}, {0, 4}, {10, 18}, {13, 26}, {15, 23}, {}}},
    {{{16, 19}, {16, 19}, {41, 44}, {41, 46}, {24, 27}, {40, 43}, {}, {38, 41}, {39, 41}, {48, 52}, {25, 26}, {25, 26}, {21, 23}, {}, {43, 46}, {6, 9}, {43, 47}, {52, 54}, {34, 39}, {26, 29}, {9, 12}, {}, {35, 38}, {}, {}, {48, 53}, {33, 35}, {}, {}, {}, {}, {40, 42}, {39, 40}, {34, 37}, {}, {22, 22}, {40, 42}, {24, 26}, {30, 33}, {}, {}, {35, 41}, {41, 48}, {34, 37}, {37, 41}, {24, 26}, {20, 22}, {34, 37}, {30, 33}, {35, 38}, {32, 35}, {30, 34}, {34, 38}, {26, 27}, {28, 29}, {28, 30}, {22, 24}, {39, 43}, {40, 42}, {38, 39}, {32, 34}, {26, 29}, {37, 38}, {45, 48}, {49, 53}, {44, 47}, {31, 35}, {}, {53, 55}, {}, {48, 54}, {43, 47}, {52, 55}, {}, {}, {}, {}, {35, 38}, {}, {21, 22}, {29, 32}, {31, 34}, {52, 55}, {}, {}, {46, 50}, {}, {46, 51}, {45, 48}, {50, 52}, {36, 40}, {41, 44}, {30, 31}, {51, 55}, {}, {}, {32, 34}, {28, 32}, {53, 55}, {48, 51}, {}, {40, 45}, {38, 40}, {45, 47}, {35, 38}, {47, 52}, {52, 55}, {48, 51}, {41, 45}, {}, {39, 44}, {}, {}, {23, 25}, {26, 27}, {}, {}, {19, 22}, {31, 34}, {32, 35}, {37, 41}, {}, {}, {}, {34, 37}, {39, 42}, {}, {}, {}, {44, 47}, {62, 64}, {35, 36}, {48, 51}, {54, 61}, {52, 56}, {}, {47, 51}, {}, {42, 45}, {46, 51}, {35, 35}, {}, {}, {50, 54}, {34, 36}, {40, 44}, {}, {47, 49}, {34, 36}, {}, {41, 46}, {}, {}, {61, 68}, {54, 56}, {26, 29}, {}, {}, {44, 46}, {}, {40, 44}, {32, 35}, {27, 30}, {46, 48}, {}, {}, {31, 39}, {36, 41}, {}, {34, 37}, {34, 37}, {35, 37}, {}, {}, {}, {}, {20, 22}, {}, {44, 48}, {56, 60}, {26, 29}, {}, {4, 7}, {}, {17, 20}, {43, 46}, {}, {}, {}, {43, 46}, {46, 50}, {36, 40}, {}, {}, {12, 13}, {10, 12}, {}, {}, {9, 12}, {}, {}, {33, 36}, {29, 31}, {11, 14}, {10, 13}, {40, 43}, {}, {47, 50}, {}}},
    {{{20, 23}, {20, 23}, {56, 64}, {57, 63}, {}, {51, 56}, {}, {}, {52, 59}, {69, 77}, {}, {}, {}, {21, 28}, {51, 58}, {14, 17}, {60, 64}, {}, {40, 46}, {}, {17, 20}, {}, {45, 49}, {}, {}, {62, 68}, {42, 47}, {}, {}, {}, {}, {54, 61}, {52, 57}, {}, {}, {23, 26}, {60, 65}, {35, 39}, {50, 53}, {41, 45}, {19, 23}, {49, 56}, {61, 66}, {44, 48}, {46, 54}, {}, {}, {51, 57}, {46, 51}, {50, 56}, {49, 55}, {}, {58, 61}, {39, 42}, {35, 39}, {36, 39}, {34, 36}, {55, 60}, {53, 56}, {52, 57}, {}, {}, {50, 53}, {64, 70}, {65, 73}, {62, 67}, {}, {}, {69, 76}, {}, {65, 73}, {65, 70}, {70, 77}, {43, 51}, {41, 49}, {}, {}, {58, 65}, {}, {29, 32}, {40, 48}, {44, 49}, {73, 81}, {}, {}, {58, 64}, {33, 39}, {64, 71}, {64, 68}, {61, 69}, {53, 58}, {}, {37, 41}, {73, 80}, {}, {23, 30}, {39, 46}, {42, 47}, {}, {}, {53, 60}, {56, 64}, {55, 60}, {}, {}, {}, {73, 79}, {66, 73}, {61, 66}, {24, 27}, {63, 71}, {45, 53}, {34, 41}, {34, 38}, {32, 37}, {56, 64}, {32, 36}, {23, 27}, {41, 45}, {41, 46}, {49, 53}, {}, {32, 35}, {44, 51}, {51, 56}, {61, 67}, {}, {20, 27}, {41, 49}, {}, {}, {}, {}, {}, {}, {63, 72}, {66, 72}, {36, 42}, {53, 58}, {64, 72}, {}, {}, {22, 28}, {71, 79}, {45, 48}, {}, {}, {64, 73}, {43, 48}, {45, 50}, {}, {}, {}, {}, {}, {30, 36}, {}, {16, 25}, {60, 66}, {}, {57, 65}, {46, 51}, {}, {58, 66}, {}, {}, {}, {}, {}, {51, 55}, {}, {50, 56}, {}, {}, {}, {}, {29, 35}, {}, {65, 69}, {}, {}, {}, {12, 15}, {}, {}, {54, 62}, {27, 36}, {30, 36}, {11, 16}, {60, 65}, {73, 80}, {54, 58}, {}, {12, 13}, {}, {}, {}, {}, {}, {}, {}, {47, 52}, {}, {15, 18}, {}, {60, 66}, {70, 77}, {72, 78}, {4, 7}}},
    {{{24, 27}, {24, 27}, {65, 71}, {64, 68}, {}, {57, 62}, {}, {}, {}, {}, {37, 42}, {37, 42}, {33, 38}, {}, {}, {18, 21}, {65, 72}, {}, {47, 54}, {}, {21, 24}, {}, {50, 55}, {}, {}, {69, 76}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {27, 29}, {66, 71}, {40, 44}, {54, 58}, {46, 51}, {24, 27}, {57, 62}, {67, 71}, {49, 56}, {55, 57}, {}, {}, {}, {52, 59}, {57, 62}, {56, 61}, {46, 51}, {62, 67}, {43, 48}, {40, 43}, {40, 45}, {37, 40}, {61, 69}, {}, {58, 65}, {}, {}, {}, {}, {}, {68, 74}, {}, {}, {77, 87}, {}, {}, {}, {}, {}, {}, {0, 6}, {0, 6}, {66, 72}, {}, {33, 33}, {49, 49}, {}, {}, {}, {}, {}, {}, {}, {}, {70, 75}, {59, 63}, {}, {}, {}, {}, {}, {}, {48, 53}, {}, {}, {61, 68}, {}, {}, {}, {}, {}, {80, 86}, {}, {}, {28, 29}, {72, 77}, {54, 60}, {}, {39, 45}, {}, {}, {37, 43}, {28, 31}, {46, 48}, {47, 50}, {}, {}, {36, 38}, {52, 59}, {}, {}, {}, {}, {}, {67, 74}, {}, {}, {}, {}, {}, {73, 78}, {73, 78}, {43, 48}, {}, {}, {}, {}, {}, {80, 89}, {49, 52}, {}, {}, {74, 81}, {49, 51}, {}, {}, {}, {}, {}, {}, {37, 41}, {}, {}, {67, 72}, {}, {}, {}, {}, {67, 73}, {}, {}, {}, {}, {}, {}, {}, {57, 62}, {}, {10, 19}, {}, {}, {}, {}, {70, 77}, {}, {}, {}, {16, 19}, {}, {}, {}, {}, {}, {}, {}, {}, {59, 65}, {}, {14, 15}, {14, 17}, {13, 14}, {}, {}, {}, {}, {}, {}, {}, {19, 22}, {}, {}, {78, 87}, {79, 84}, {8, 11}}},
    {{{28, 31}, {28, 31}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {23, 26}, {}, {}, {}, {}, {}}},
    {{{32, 35}, {32, 35}, {72, 79}, {69, 78}, {39, 44}, {63, 72}, {}, {58, 65}, {60, 70}, {78, 87}, {43, 48}, {43, 48}, {39, 44}, {29, 36}, {59, 67}, {22, 27}, {73, 82}, {55, 62}, {55, 65}, {39, 48}, {25, 29}, {}, {56, 63}, {5, 9}, {8, 13}, {85, 95}, {48, 57}, {45, 54}, {17, 20}, {}, {}, {62, 71}, {58, 64}, {55, 63}, {22, 29}, {30, 35}, {72, 78}, {45, 48}, {59, 64}, {52, 59}, {28, 32}, {}, {72, 82}, {57, 61}, {58, 64}, {31, 38}, {26, 31}, {58, 62}, {60, 68}, {63, 67}, {62, 68}, {52, 57}, {}, {49, 56}, {44, 48}, {46, 49}, {41, 46}, {70, 74}, {57, 63}, {66, 74}, {44, 54}, {43, 49}, {54, 63}, {71, 80}, {74, 84}, {75, 84}, {48, 55}, {5, 9}, {96, 102}, {47, 57}, {74, 84}, {71, 81}, {78, 87}, {52, 58}, {50, 57}, {7, 9}, {7, 8}, {85, 94}, {}, {34, 36}, {50, 56}, {}, {82, 91}, {}, {}, {65, 76}, {40, 48}, {72, 81}, {69, 78}, {76, 79}, {64, 69}, {60, 66}, {42, 49}, {81, 90}, {}, {31, 35}, {47, 52}, {54, 60}, {56, 65}, {58, 66}, {75, 83}, {78, 87}, {61, 67}, {48, 59}, {53, 61}, {72, 81}, {95, 103}, {74, 83}, {67, 75}, {30, 35}, {78, 86}, {61, 68}, {42, 49}, {57, 61}, {38, 43}, {65, 70}, {44, 51}, {32, 37}, {49, 55}, {63, 69}, {66, 72}, {39, 45}, {39, 41}, {60, 67}, {57, 65}, {68, 77}, {}, {28, 31}, {50, 57}, {75, 86}, {76, 85}, {37, 44}, {57, 63}, {70, 81}, {57, 71}, {87, 99}, {79, 90}, {49, 60}, {59, 65}, {73, 82}, {50, 59}, {}, {29, 37}, {96, 105}, {53, 59}, {59, 68}, {}, {90, 99}, {52, 59}, {51, 65}, {59, 68}, {}, {}, {82, 91}, {57, 64}, {42, 51}, {}, {26, 34}, {77, 85}, {}, {66, 73}, {52, 59}, {41, 50}, {74, 83}, {}, {}, {49, 54}, {42, 52}, {51, 61}, {56, 64}, {49, 60}, {63, 66}, {9, 13}, {20, 23}, {}, {0, 2}, {36, 43}, {11, 14}, {78, 87}, {70, 80}, {39, 47}, {15, 18}, {20, 24}, {}, {21, 28}, {63, 72}, {37, 48}, {37, 45}, {17, 20}, {66, 74}, {81, 90}, {74, 82}, {29, 38}, {16, 19}, {18, 21}, {15, 18}, {45, 53}, {7, 15}, {13, 17}, {17, 20}, {10, 14}, {53, 61}, {40, 49}, {}, {14, 18}, {67, 72}, {88, 92}, {85, 87}, {12, 17}}},
    {{{36, 43}, {36, 43}, {}, {88, 99}, {51, 57}, {76, 79}, {}, {76, 86}, {}, {98, 107}, {74, 81}, {74, 81}, {54, 60}, {37, 43}, {}, {28, 33}, {92, 100}, {63, 70}, {}, {55, 64}, {30, 34}, {15, 34}, {67, 74}, {10, 14}, {}, {108, 118}, {67, 73}, {115, 124}, {21, 25}, {10, 14}, {10, 14}, {}, {72, 76}, {76, 85}, {34, 39}, {36, 42}, {82, 88}, {53, 58}, {65, 69}, {60, 65}, {33, 36}, {63, 68}, {83, 91}, {62, 68}, {}, {}, {}, {}, {69, 74}, {68, 72}, {69, 73}, {58, 62}, {71, 75}, {60, 64}, {49, 53}, {50, 54}, {47, 52}, {75, 81}, {}, {80, 85}, {66, 72}, {60, 66}, {}, {93, 102}, {}, {96, 104}, {62, 71}, {10, 14}, {115, 126}, {99, 108}, {}, {95, 107}, {97, 106}, {62, 67}, {62, 69}, {10, 11}, {9, 10}, {109, 116}, {}, {39, 42}, {65, 71}, {}, {98, 107}, {}, {5, 19}, {}, {56, 62}, {}, {90, 99}, {}, {79, 87}, {76, 82}, {}, {103, 107}, {}, {36, 39}, {}, {69, 76}, {66, 74}, {67, 75}, {87, 95}, {90, 97}, {81, 85}, {60, 67}, {72, 79}, {91, 100}, {112, 122}, {98, 109}, {89, 98}, {39, 44}, {94, 100}, {69, 74}, {52, 58}, {72, 78}, {44, 48}, {}, {52, 56}, {38, 45}, {56, 62}, {76, 82}, {79, 86}, {58, 67}, {45, 51}, {75, 82}, {}, {94, 105}, {}, {32, 38}, {62, 69}, {99, 109}, {86, 92}, {45, 49}, {64, 72}, {82, 90}, {72, 78}, {107, 116}, {96, 102}, {}, {}, {95, 106}, {66, 74}, {}, {38, 45}, {112, 121}, {60, 64}, {78, 86}, {}, {105, 114}, {60, 64}, {}, {79, 86}, {}, {}, {92, 100}, {65, 75}, {}, {}, {35, 40}, {93, 100}, {0, 4}, {}, {69, 74}, {}, {87, 96}, {5, 8}, {}, {}, {63, 72}, {69, 77}, {}, {}, {75, 80}, {}, {24, 31}, {0, 6}, {}, {46, 52}, {15, 20}, {99, 106}, {81, 90}, {53, 60}, {19, 24}, {25, 29}, {15, 34}, {37, 44}, {}, {49, 60}, {46, 52}, {}, {86, 93}, {102, 112}, {95, 104}, {102, 111}, {}, {}, {}, {}, {16, 21}, {18, 22}, {21, 25}, {}, {}, {}, {27, 30}, {19, 22}, {79, 89}, {93, 98}, {88, 93}, {18, 22}}},
    {{{44, 48}, {44, 48}, {}, {100, 104}, {}, {}, {}, {98, 102}, {}, {}, {82, 86}, {82, 86}, {61, 63}, {}, {}, {}, {}, {71, 76}, {}, {}, {35, 39}, {}, {}, {}, {}, {119, 124}, {}, {125, 129}, {}, {}, {}, {}, {113, 116}, {86, 91}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 59}, {}, {}, {}, {}, {67, 72}, {}, {}, {}, {105, 110}, {}, {}, {127, 131}, {109, 116}, {}, {108, 113}, {}, {}, {}, {}, {}, {}, {}, {}, {72, 76}, {}, {}, {0, 4}, {}, {}, {}, {}, {}, {}, {88, 92}, {83, 87}, {}, {}, {}, {}, {}, {}, {75, 80}, {}, {96, 100}, {}, {}, {68, 72}, {80, 84}, {101, 108}, {123, 128}, {}, {}, {}, {101, 104}, {}, {}, {83, 87}, {53, 56}, {}, {61, 67}, {}, {}, {}, {}, {74, 79}, {}, {}, {}, {}, {}, {}, {}, {110, 117}, {93, 97}, {50, 55}, {73, 78}, {}, {79, 83}, {117, 120}, {}, {}, {}, {}, {75, 79}, {}, {}, {122, 126}, {69, 72}, {87, 91}, {}, {115, 120}, {}, {}, {92, 96}, {}, {}, {101, 105}, {}, {}, {}, {}, {101, 104}, {}, {}, {75, 78}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {104, 109}, {}, {}, {}, {}, {}, {}, {107, 112}, {}, {}, {}, {30, 34}, {}, {}, {}, {}, {}, {}, {}, {113, 122}, {105, 110}, {112, 117}, {}, {}, {}, {}, {22, 26}, {}, {}, {}, {}, {}, {31, 38}, {}, {}, {99, 103}, {94, 101}, {23, 27}}},
    {{{49, 52}, {49, 52}, {126, 139}, {111, 122}, {}, {110, 124}, {}, {}, {83, 92}, {114, 124}, {}, {}, {}, {50, 58}, {103, 111}, {40, 45}, {107, 117}, {}, {74, 84}, {}, {40, 43}, {}, {104, 111}, {}, {21, 24}, {125, 133}, {78, 88}, {}, {}, {}, {}, {82, 94}, {117, 126}, {}, {51, 59}, {48, 54}, {97, 107}, {64, 73}, {77, 84}, {75, 87}, {37, 43}, {77, 90}, {100, 112}, {74, 89}, {71, 79}, {}, {}, {109, 119}, {103, 118}, {110, 127}, {109, 113}, {70, 83}, {85, 95}, {72, 82}, {61, 68}, {59, 68}, {60, 68}, {87, 95}, {73, 78}, {94, 105}, {}, {}, {106, 114}, {109, 119}, {96, 106}, {111, 122}, {}, {}, {132, 145}, {}, {95, 108}, {114, 124}, {114, 125}, {72, 85}, {75, 85}, {}, {}, {123, 128}, {}, {47, 51}, {85, 95}, {90, 101}, {117, 131}, {}, {}, {123, 136}, {69, 80}, {97, 107}, {105, 116}, {122, 129}, {93, 98}, {}, {68, 72}, {114, 126}, {}, {46, 55}, {91, 99}, {82, 90}, {}, {}, {101, 111}, {103, 114}, {90, 95}, {}, {}, {}, {129, 138}, {118, 131}, {106, 116}, {53, 59}, {112, 122}, {82, 92}, {66, 76}, {97, 104}, {63, 70}, {106, 113}, {76, 87}, {53, 60}, {71, 80}, {95, 102}, {95, 99}, {}, {61, 69}, {109, 118}, {110, 120}, {112, 123}, {}, {46, 55}, {75, 85}, {126, 138}, {}, {}, {}, {}, {}, {121, 131}, {112, 125}, {76, 90}, {76, 86}, {112, 120}, {}, {}, {51, 62}, {127, 136}, {80, 86}, {}, {}, {121, 130}, {72, 84}, {72, 86}, {}, {}, {}, {}, {}, {57, 64}, {}, {48, 58}, {105, 113}, {}, {121, 131}, {79, 88}, {}, {122, 133}, {}, {}, {}, {}, {}, {112, 122}, {}, {110, 119}, {}, {32, 35}, {}, {6, 7}, {97, 108}, {}, {113, 121}, {}, {}, {}, {35, 38}, {}, {}, {103, 113}, {68, 79}, {61, 72}, {25, 30}, {99, 110}, {131, 142}, {111, 121}, {}, {23, 25}, {27, 30}, {22, 24}, {}, {}, {}, {}, {}, {110, 121}, {}, {39, 42}, {}, {137, 147}, {110, 117}, {110, 121}, {28, 33}}},
    {{{53, 56}, {53, 56}, {140, 151}, {123, 130}, {}, {125, 133}, {}, {}, {93, 102}, {125, 134}, {87, 92}, {87, 92}, {64, 68}, {59, 68}, {112, 120}, {46, 51}, {118, 126}, {}, {85, 95}, {}, {44, 47}, {}, {112, 120}, {}, {}, {134, 141}, {89, 99}, {}, {31, 36}, {}, {}, {95, 104}, {127, 135}, {}, {}, {55, 57}, {108, 114}, {74, 84}, {85, 92}, {88, 97}, {44, 46}, {91, 100}, {113, 123}, {90, 102}, {80, 85}, {}, {}, {120, 130}, {119, 129}, {128, 137}, {114, 118}, {84, 93}, {96, 104}, {83, 89}, {69, 78}, {69, 73}, {69, 76}, {96, 104}, {79, 82}, {106, 115}, {}, {}, {115, 122}, {120, 129}, {107, 116}, {123, 130}, {}, {}, {146, 158}, {}, {109, 118}, {125, 134}, {126, 137}, {86, 96}, {86, 93}, {}, {}, {129, 135}, {}, {52, 54}, {96, 103}, {102, 109}, {132, 138}, {}, {}, {137, 143}, {81, 91}, {108, 113}, {117, 126}, {130, 138}, {99, 103}, {}, {73, 75}, {127, 136}, {}, {56, 64}, {100, 108}, {91, 94}, {}, {}, {112, 120}, {115, 122}, {96, 101}, {}, {}, {}, {139, 148}, {132, 143}, {117, 125}, {60, 65}, {123, 131}, {93, 97}, {77, 83}, {105, 110}, {71, 75}, {114, 122}, {88, 94}, {61, 70}, {81, 87}, {103, 109}, {100, 106}, {}, {70, 75}, {119, 128}, {121, 129}, {124, 134}, {}, {56, 62}, {86, 93}, {139, 150}, {}, {}, {}, {}, {}, {132, 143}, {126, 135}, {91, 102}, {87, 95}, {121, 126}, {}, {}, {63, 71}, {137, 144}, {87, 92}, {}, {}, {131, 138}, {85, 92}, {87, 95}, {}, {}, {}, {}, {}, {65, 73}, {}, {59, 66}, {114, 119}, {}, {132, 140}, {89, 97}, {}, {134, 142}, {}, {}, {}, {}, {}, {123, 131}, {}, {120, 128}, {}, {}, {}, {}, {109, 117}, {}, {122, 131}, {}, {}, {30, 34}, {39, 42}, {}, {}, {114, 124}, {80, 88}, {73, 81}, {31, 36}, {111, 122}, {143, 153}, {122, 129}, {}, {26, 28}, {31, 34}, {}, {}, {}, {}, {31, 35}, {}, {122, 132}, {}, {43, 46}, {}, {148, 158}, {118, 128}, {122, 129}, {34, 39}}},
    {{{57, 60}, {57, 60}, {}, {131, 135}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {69, 76}, {}, {}, {128, 134}, {}, {}, {}, {}, {}, {121, 127}, {}, {}, {142, 150}, {}, {}, {37, 40}, {}, {}, {}, {}, {}, {}, {}, {115, 120}, {}, {}, {100, 104}, {47, 51}, {105, 108}, {127, 130}, {103, 109}, {86, 89}, {}, {66, 69}, {131, 134}, {130, 136}, {140, 145}, {119, 123}, {94, 98}, {105, 111}, {94, 96}, {79, 83}, {77, 80}, {77, 81}, {109, 112}, {}, {118, 123}, {}, {}, {}, {}, {}, {131, 137}, {}, {}, {160, 170}, {}, {}, {}, {}, {}, {}, {}, {}, {142, 145}, {}, {}, {104, 108}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {104, 110}, {}, {}, {}, {}, {65, 68}, {}, {95, 98}, {}, {}, {}, {}, {}, {}, {}, {}, {159, 163}, {}, {}, {}, {132, 137}, {100, 101}, {}, {112, 118}, {78, 79}, {}, {95, 101}, {}, {}, {}, {108, 111}, {}, {}, {}, {}, {}, {}, {65, 72}, {}, {151, 158}, {}, {}, {}, {}, {}, {144, 155}, {}, {}, {}, {}, {}, {}, {}, {146, 153}, {}, {}, {}, {139, 145}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {120, 127}, {}, {}, {}, {}, {146, 153}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {132, 137}, {}, {}, {35, 38}, {}, {}, {}, {}, {92, 95}, {82, 88}, {}, {}, {}, {131, 138}, {}, {}, {35, 37}, {25, 27}, {}, {}, {}, {36, 39}, {}, {}, {}, {}, {}, {}, {129, 135}, {130, 136}, {}}},
    {{{61, 63}, {61, 62}, {152, 155}, {136, 142}, {61, 64}, {134, 138}, {}, {123, 128}, {103, 110}, {135, 142}, {93, 95}, {93, 95}, {69, 71}, {}, {121, 125}, {52, 55}, {135, 141}, {83, 88}, {96, 104}, {71, 75}, {48, 51}, {}, {128, 133}, {}, {}, {151, 157}, {100, 102}, {}, {}, {}, {}, {105, 109}, {136, 140}, {92, 95}, {60, 67}, {58, 59}, {121, 129}, {90, 94}, {97, 104}, {105, 110}, {52, 57}, {109, 114}, {131, 138}, {}, {}, {72, 80}, {70, 75}, {135, 140}, {137, 144}, {146, 152}, {124, 129}, {99, 106}, {112, 119}, {97, 99}, {84, 89}, {81, 86}, {82, 84}, {113, 119}, {83, 87}, {124, 127}, {81, 88}, {90, 92}, {123, 125}, {130, 133}, {117, 123}, {138, 140}, {79, 83}, {}, {171, 176}, {}, {119, 124}, {135, 142}, {138, 143}, {97, 100}, {}, {}, {}, {146, 149}, {}, {55, 55}, {109, 113}, {110, 117}, {139, 143}, {}, {}, {144, 151}, {92, 96}, {114, 121}, {127, 134}, {139, 145}, {111, 117}, {104, 110}, {76, 78}, {137, 143}, {}, {}, {109, 113}, {99, 106}, {88, 94}, {80, 86}, {130, 137}, {}, {102, 107}, {73, 80}, {98, 103}, {139, 146}, {164, 173}, {144, 150}, {126, 132}, {66, 68}, {138, 144}, {102, 104}, {}, {119, 120}, {80, 82}, {123, 127}, {102, 103}, {71, 76}, {}, {110, 114}, {}, {107, 112}, {}, {}, {130, 135}, {135, 141}, {}, {}, {}, {159, 164}, {104, 108}, {56, 58}, {79, 84}, {98, 104}, {91, 99}, {156, 161}, {136, 141}, {}, {96, 101}, {127, 131}, {95, 102}, {}, {}, {154, 159}, {93, 95}, {109, 116}, {}, {146, 150}, {93, 94}, {96, 101}, {115, 121}, {}, {}, {112, 117}, {81, 88}, {74, 81}, {}, {}, {128, 131}, {}, {141, 147}, {98, 103}, {66, 71}, {154, 157}, {}, {}, {}, {78, 83}, {}, {132, 137}, {73, 77}, {129, 132}, {}, {}, {}, {}, {118, 121}, {}, {138, 144}, {100, 105}, {67, 71}, {}, {43, 46}, {}, {51, 56}, {125, 130}, {}, {}, {}, {123, 129}, {154, 158}, {139, 143}, {}, {}, {38, 41}, {28, 30}, {}, {}, {28, 31}, {}, {}, {133, 137}, {50, 55}, {47, 50}, {28, 31}, {159, 164}, {136, 140}, {137, 142}, {}}},
    {{{64, 65}, {63, 64}, {156, 158}, {143, 146}, {}, {139, 142}, {}, {129, 131}, {111, 115}, {143, 148}, {96, 97}, {96, 97}, {72, 72}, {}, {126, 127}, {}, {142, 145}, {89, 92}, {105, 106}, {}, {52, 53}, {}, {134, 137}, {}, {}, {}, {103, 105}, {130, 133}, {41, 41}, {}, {}, {110, 112}, {141, 143}, {96, 97}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {88, 91}, {128, 130}, {}, {93, 95}, {126, 130}, {}, {124, 129}, {141, 146}, {}, {}, {}, {117, 120}, {125, 131}, {143, 147}, {144, 146}, {101, 105}, {94, 96}, {}, {}, {150, 151}, {}, {56, 57}, {}, {118, 120}, {144, 147}, {}, {}, {152, 154}, {97, 97}, {122, 126}, {135, 138}, {146, 149}, {118, 121}, {111, 113}, {79, 79}, {144, 146}, {}, {}, {114, 115}, {107, 108}, {95, 99}, {87, 88}, {}, {123, 125}, {108, 110}, {81, 84}, {104, 106}, {147, 151}, {}, {151, 156}, {133, 136}, {69, 70}, {145, 146}, {105, 106}, {84, 85}, {121, 122}, {83, 84}, {128, 132}, {104, 105}, {77, 78}, {88, 89}, {115, 117}, {112, 113}, {113, 116}, {76, 78}, {129, 131}, {136, 138}, {142, 145}, {}, {}, {94, 96}, {165, 170}, {109, 111}, {59, 60}, {85, 87}, {105, 106}, {100, 104}, {}, {142, 148}, {103, 107}, {102, 105}, {132, 136}, {103, 106}, {}, {}, {}, {96, 97}, {117, 121}, {}, {}, {95, 97}, {102, 104}, {122, 124}, {}, {}, {}, {89, 92}, {82, 83}, {}, {}, {}, {}, {148, 151}, {104, 108}, {}, {158, 160}, {}, {}, {64, 67}, {}, {99, 102}, {138, 140}, {78, 81}, {133, 134}, {19, 19}, {}, {}, {}, {122, 124}, {}, {145, 148}, {106, 108}, {}, {39, 39}, {47, 48}, {}, {57, 58}, {131, 134}, {}, {}, {41, 42}, {130, 132}, {159, 162}, {}, {118, 121}, {29, 29}, {42, 43}, {31, 32}, {64, 67}, {}, {32, 33}, {40, 40}, {19, 20}, {138, 141}, {56, 57}, {}, {32, 33}, {165, 167}, {}, {}, {}}},
    {{{66, 67}, {65, 66}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {51, 52}, {}, {}, {}, {}, {}}},
    {{{68, 69}, {67, 68}, {202, 202}, {185, 186}, {102, 103}, {173, 177}, {}, {173, 174}, {166, 168}, {185, 186}, {130, 131}, {130, 131}, {111, 113}, {77, 78}, {157, 158}, {}, {181, 182}, {128, 130}, {158, 160}, {118, 120}, {91, 92}, {}, {183, 184}, {}, {33, 34}, {206, 207}, {106, 107}, {134, 136}, {42, 42}, {}, {}, {163, 166}, {176, 177}, {130, 132}, {112, 116}, {95, 96}, {162, 164}, {129, 131}, {144, 146}, {155, 157}, {83, 84}, {152, 154}, {191, 193}, {165, 167}, {124, 125}, {125, 126}, {108, 109}, {180, 182}, {172, 173}, {189, 191}, {165, 167}, {139, 140}, {158, 160}, {130, 130}, {133, 136}, {121, 123}, {107, 108}, {150, 151}, {97, 99}, {159, 160}, {126, 128}, {131, 133}, {175, 177}, {175, 176}, {182, 185}, {187, 189}, {127, 128}, {}, {224, 225}, {121, 124}, {172, 175}, {187, 188}, {185, 187}, {106, 110}, {97, 98}, {}, {}, {188, 189}, {}, {87, 88}, {155, 156}, {165, 167}, {183, 187}, {}, {}, {202, 203}, {98, 100}, {172, 173}, {181, 183}, {183, 186}, {157, 158}, {149, 151}, {108, 108}, {188, 190}, {10, 11}, {69, 70}, {148, 149}, {142, 143}, {145, 147}, {114, 115}, {210, 212}, {170, 171}, {146, 147}, {123, 125}, {143, 145}, {193, 195}, {238, 240}, {196, 198}, {179, 181}, {102, 103}, {188, 189}, {147, 148}, {123, 124}, {158, 159}, {116, 117}, {186, 187}, {146, 147}, {112, 113}, {123, 124}, {159, 160}, {148, 150}, {154, 156}, {110, 111}, {165, 166}, {184, 186}, {193, 196}, {9, 10}, {73, 75}, {97, 98}, {211, 214}, {144, 146}, {89, 92}, {116, 117}, {136, 137}, {151, 154}, {225, 226}, {191, 193}, {115, 117}, {149, 151}, {180, 181}, {138, 140}, {}, {79, 80}, {204, 205}, {136, 136}, {169, 170}, {10, 11}, {188, 189}, {133, 134}, {147, 149}, {158, 159}, {9, 10}, {}, {155, 157}, {124, 126}, {121, 122}, {}, {75, 76}, {167, 167}, {}, {196, 198}, {145, 147}, {115, 116}, {194, 196}, {}, {}, {75, 76}, {125, 126}, {132, 136}, {186, 188}, {133, 135}, {}, {25, 25}, {}, {}, {}, {159, 161}, {}, {187, 190}, {154, 155}, {100, 101}, {40, 40}, {}, {}, {97, 98}, {176, 178}, {96, 100}, {89, 93}, {47, 48}, {175, 177}, {204, 207}, {196, 197}, {122, 123}, {34, 34}, {84, 85}, {}, {76, 78}, {27, 28}, {64, 65}, {41, 42}, {26, 26}, {191, 193}, {63, 64}, {}, {72, 73}, {214, 216}, {185, 187}, {189, 192}, {}}},
    {{{70, 71}, {69, 70}, {203, 205}, {187, 188}, {104, 107}, {178, 183}, {}, {175, 179}, {169, 173}, {187, 190}, {132, 136}, {132, 136}, {114, 116}, {79, 81}, {159, 162}, {91, 94}, {183, 187}, {131, 134}, {161, 165}, {121, 124}, {93, 94}, {35, 36}, {185, 188}, {}, {35, 37}, {208, 212}, {108, 114}, {137, 139}, {43, 44}, {}, {}, {167, 169}, {178, 181}, {133, 136}, {117, 121}, {97, 99}, {165, 167}, {132, 135}, {147, 149}, {158, 161}, {85, 87}, {155, 159}, {194, 198}, {168, 172}, {126, 128}, {127, 131}, {110, 114}, {183, 186}, {174, 179}, {192, 196}, {168, 171}, {141, 143}, {161, 165}, {131, 134}, {137, 139}, {124, 126}, {109, 113}, {152, 155}, {100, 104}, {161, 164}, {129, 133}, {134, 137}, {178, 181}, {177, 181}, {186, 188}, {190, 194}, {129, 132}, {}, {226, 229}, {125, 129}, {176, 181}, {189, 194}, {188, 192}, {111, 116}, {99, 102}, {}, {}, {190, 193}, {}, {89, 90}, {157, 158}, {168, 171}, {188, 193}, {}, {20, 21}, {204, 208}, {101, 103}, {174, 179}, {184, 187}, {187, 189}, {159, 162}, {152, 154}, {109, 111}, {191, 195}, {12, 12}, {71, 73}, {150, 153}, {144, 146}, {148, 152}, {116, 118}, {213, 217}, {172, 175}, {148, 151}, {126, 130}, {146, 148}, {196, 201}, {241, 245}, {199, 202}, {182, 185}, {104, 104}, {190, 191}, {149, 150}, {125, 126}, {160, 161}, {}, {188, 192}, {148, 149}, {114, 116}, {125, 126}, {161, 163}, {151, 154}, {157, 161}, {112, 112}, {167, 170}, {187, 190}, {197, 202}, {11, 12}, {76, 79}, {99, 102}, {215, 219}, {147, 150}, {93, 96}, {118, 121}, {138, 141}, {155, 159}, {227, 230}, {194, 197}, {118, 123}, {152, 155}, {182, 185}, {141, 144}, {}, {81, 84}, {206, 209}, {137, 138}, {171, 173}, {12, 13}, {190, 193}, {135, 136}, {150, 152}, {160, 163}, {11, 12}, {19, 20}, {}, {127, 130}, {123, 126}, {}, {77, 80}, {168, 169}, {}, {199, 202}, {148, 151}, {117, 120}, {197, 201}, {}, {}, {77, 81}, {127, 131}, {137, 141}, {189, 192}, {136, 139}, {168, 169}, {26, 27}, {}, {}, {}, {162, 163}, {}, {191, 194}, {156, 160}, {102, 104}, {41, 41}, {}, {35, 36}, {99, 103}, {179, 183}, {101, 106}, {94, 97}, {49, 50}, {178, 181}, {208, 213}, {198, 200}, {124, 127}, {35, 36}, {86, 87}, {}, {79, 83}, {29, 32}, {66, 66}, {43, 44}, {27, 28}, {194, 197}, {65, 69}, {}, {74, 75}, {217, 219}, {188, 191}, {193, 196}, {}}},
    {{{72, 75}, {71, 74}, {236, 242}, {208, 214}, {}, {205, 213}, {}, {233, 237}, {209, 216}, {224, 227}, {}, {}, {}, {}, {193, 197}, {95, 99}, {203, 208}, {}, {187, 194}, {}, {95, 98}, {}, {224, 231}, {}, {38, 41}, {221, 226}, {145, 150}, {}, {}, {}, {}, {209, 215}, {211, 216}, {}, {}, {106, 110}, {178, 182}, {144, 150}, {157, 163}, {170, 176}, {98, 102}, {170, 177}, {211, 217}, {182, 186}, {136, 141}, {}, {}, {196, 201}, {180, 186}, {206, 212}, {198, 204}, {171, 178}, {198, 205}, {163, 169}, {172, 176}, {135, 142}, {122, 127}, {182, 189}, {130, 133}, {176, 180}, {}, {181, 189}, {217, 220}, {211, 214}, {229, 234}, {217, 221}, {}, {}, {233, 238}, {}, {214, 219}, {215, 220}, {216, 222}, {147, 154}, {127, 134}, {}, {}, {231, 234}, {}, {107, 110}, {186, 189}, {}, {224, 230}, {}, {}, {241, 247}, {104, 108}, {215, 223}, {226, 231}, {216, 221}, {178, 182}, {178, 181}, {131, 137}, {232, 237}, {}, {}, {181, 188}, {175, 180}, {}, {}, {232, 237}, {199, 206}, {178, 181}, {}, {203, 210}, {249, 254}, {255, 6}, {244, 249}, {215, 222}, {128, 132}, {231, 236}, {175, 179}, {139, 143}, {180, 183}, {142, 147}, {203, 207}, {158, 163}, {126, 130}, {149, 154}, {185, 189}, {183, 188}, {}, {132, 136}, {198, 205}, {215, 220}, {243, 249}, {}, {}, {127, 134}, {236, 242}, {}, {}, {}, {}, {}, {234, 241}, {208, 214}, {135, 141}, {192, 196}, {217, 222}, {172, 179}, {}, {}, {232, 237}, {163, 164}, {200, 204}, {}, {218, 224}, {152, 158}, {189, 196}, {225, 233}, {}, {}, {}, {}, {127, 134}, {}, {}, {179, 182}, {}, {232, 238}, {162, 168}, {}, {233, 243}, {}, {}, {}, {}, {205, 210}, {221, 226}, {}, {198, 205}, {}, {36, 38}, {}, {}, {182, 187}, {}, {225, 229}, {}, {}, {}, {85, 88}, {}, {}, {202, 208}, {}, {}, {51, 55}, {212, 219}, {233, 238}, {203, 209}, {}, {37, 39}, {}, {}, {}, {}, {}, {}, {}, {232, 239}, {}, {87, 90}, {}, {236, 240}, {201, 212}, {205, 211}, {45, 48}}},
    {{{}, {}, {0, 4}, {0, 5}, {}, {0, 2}, {}, {0, 3}, {}, {0, 5}, {0, 1}, {0, 1}, {}, {0, 5}, {0, 4}, {}, {0, 4}, {0, 5}, {0, 7}, {}, {}, {}, {}, {}, {}, {6, 10}, {0, 2}, {7, 12}, {}, {}, {}, {0, 4}, {0, 5}, {4, 7}, {}, {0, 3}, {}, {}, {}, {0, 6}, {0, 3}, {0, 5}, {}, {0, 5}, {0, 4}, {}, {}, {}, {0, 3}, {0, 4}, {}, {0, 5}, {}, {}, {0, 3}, {0, 3}, {}, {0, 4}, {0, 3}, {0, 5}, {}, {}, {}, {0, 3}, {0, 6}, {0, 2}, {}, {}, {7, 12}, {7, 15}, {}, {}, {0, 4}, {}, {0, 4}, {}, {}, {0, 3}, {}, {0, 1}, {0, 4}, {0, 2}, {0, 5}, {}, {}, {0, 5}, {}, {0, 7}, {}, {0, 6}, {0, 3}, {0, 5}, {0, 3}, {0, 9}, {}, {0, 3}, {}, {0, 2}, {0, 4}, {0, 3}, {}, {0, 5}, {0, 2}, {0, 4}, {0, 6}, {0, 10}, {5, 10}, {0, 5}, {}, {0, 4}, {0, 5}, {0, 4}, {0, 5}, {0, 3}, {0, 3}, {0, 4}, {0, 5}, {0, 5}, {0, 3}, {0, 3}, {0, 4}, {0, 4}, {0, 2}, {0, 3}, {}, {}, {}, {0, 2}, {0, 4}, {}, {0, 4}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {}, {0, 5}, {0, 4}, {0, 5}, {0, 6}, {}, {}, {0, 3}, {6, 10}, {0, 5}, {0, 7}, {}, {5, 8}, {0, 4}, {0, 5}, {0, 5}, {}, {}, {}, {0, 5}, {0, 3}, {}, {}, {4, 7}, {}, {0, 5}, {0, 4}, {}, {0, 5}, {}, {}, {}, {}, {0, 6}, {}, {0, 6}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 7}, {}, {}, {}, {}, {}, {0, 4}, {0, 4}, {0, 7}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 4}, {0, 8}, {}}},
    {{{}, {}, {22, 30}, {23, 31}, {5, 11}, {21, 31}, {}, {16, 27}, {18, 29}, {29, 36}, {18, 21}, {18, 21}, {14, 18}, {17, 20}, {24, 39}, {}, {26, 31}, {39, 51}, {21, 33}, {9, 16}, {}, {}, {21, 28}, {}, {}, {34, 42}, {23, 28}, {27, 35}, {}, {5, 9}, {5, 9}, {}, {24, 30}, {17, 24}, {15, 21}, {16, 21}, {21, 29}, {14, 19}, {15, 20}, {24, 32}, {15, 18}, {25, 34}, {20, 27}, {18, 27}, {26, 31}, {12, 18}, {9, 16}, {14, 21}, {19, 26}, {24, 30}, {16, 23}, {21, 27}, {13, 24}, {13, 20}, {23, 27}, {21, 27}, {11, 16}, {24, 34}, {26, 30}, {25, 28}, {11, 19}, {8, 15}, {20, 24}, {24, 32}, {31, 37}, {24, 31}, {11, 18}, {}, {37, 44}, {27, 37}, {24, 32}, {22, 30}, {28, 42}, {22, 36}, {25, 35}, {}, {}, {16, 23}, {}, {11, 15}, {18, 23}, {}, {32, 42}, {}, {}, {28, 39}, {17, 27}, {}, {22, 32}, {24, 39}, {18, 25}, {20, 32}, {15, 23}, {33, 40}, {0, 4}, {17, 22}, {21, 27}, {18, 21}, {41, 52}, {29, 41}, {16, 29}, {21, 29}, {23, 30}, {36, 44}, {14, 25}, {21, 31}, {32, 44}, {28, 36}, {21, 28}, {}, {22, 32}, {20, 26}, {}, {11, 16}, {18, 20}, {24, 32}, {21, 27}, {13, 18}, {17, 24}, {15, 25}, {22, 27}, {23, 34}, {17, 19}, {23, 31}, {19, 24}, {17, 24}, {0, 3}, {16, 19}, {25, 35}, {25, 34}, {34, 50}, {26, 34}, {36, 42}, {35, 44}, {39, 51}, {30, 37}, {23, 39}, {25, 31}, {26, 34}, {29, 37}, {12, 22}, {}, {16, 21}, {31, 43}, {21, 26}, {21, 29}, {0, 4}, {30, 40}, {24, 33}, {28, 37}, {19, 31}, {0, 3}, {5, 8}, {35, 50}, {41, 53}, {13, 25}, {}, {13, 15}, {26, 38}, {}, {23, 30}, {16, 23}, {12, 15}, {25, 36}, {}, {}, {14, 21}, {14, 21}, {22, 36}, {19, 24}, {17, 22}, {17, 26}, {5, 8}, {5, 9}, {}, {}, {14, 15}, {5, 10}, {19, 31}, {37, 46}, {10, 16}, {}, {}, {}, {8, 16}, {27, 37}, {23, 26}, {23, 29}, {}, {22, 30}, {19, 25}, {24, 29}, {12, 21}, {9, 11}, {8, 11}, {7, 9}, {10, 22}, {}, {5, 8}, {}, {5, 9}, {15, 23}, {12, 19}, {}, {5, 9}, {19, 27}, {27, 36}, {24, 32}, {}}},
    {{{}, {}, {31, 34}, {}, {}, {35, 37}, {}, {}, {}, {37, 39}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {31, 32}, {}, {}, {}, {}, {}, {}, {}, {}, {33, 36}, {31, 33}, {28, 30}, {}, {}, {}, {20, 23}, {21, 22}, {}, {}, {}, {32, 35}, {}, {}, {}, {}, {}, {}, {}, {28, 31}, {}, {25, 27}, {21, 22}, {}, {}, {}, {}, {35, 36}, {}, {}, {}, {29, 32}, {33, 35}, {}, {}, {}, {}, {}, {}, {38, 42}, {31, 34}, {43, 45}, {}, {}, {}, {}, {28, 30}, {}, {16, 16}, {}, {24, 26}, {}, {}, {}, {}, {}, {38, 40}, {33, 36}, {40, 42}, {}, {}, {24, 26}, {41, 43}, {}, {}, {}, {}, {}, {44, 45}, {}, {30, 33}, {31, 34}, {}, {}, {}, {}, {37, 40}, {29, 32}, {}, {33, 35}, {30, 33}, {22, 23}, {}, {23, 25}, {37, 40}, {}, {}, {}, {29, 31}, {28, 31}, {}, {22, 23}, {}, {27, 30}, {25, 29}, {}, {}, {}, {}, {54, 58}, {}, {43, 45}, {48, 50}, {}, {}, {}, {}, {35, 37}, {38, 41}, {}, {}, {}, {}, {27, 29}, {}, {}, {}, {}, {}, {}, {}, {}, {54, 56}, {}, {}, {}, {}, {}, {}, {34, 35}, {}, {}, {40, 42}, {}, {}, {}, {}, {}, {27, 30}, {23, 26}, {}, {}, {}, {}, {}, {}, {}, {}, {50, 52}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {31, 34}, {31, 36}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {27, 29}, {20, 22}, {}, {}, {28, 31}, {41, 44}, {38, 41}, {}}},
    {{{}, {}, {35, 37}, {}, {15, 18}, {38, 39}, {}, {31, 34}, {}, {40, 43}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {20, 22}, {}, {}, {33, 34}, {}, {}, {}, {29, 32}, {}, {}, {}, {}, {}, {34, 35}, {31, 33}, {}, {}, {30, 32}, {}, {23, 25}, {}, {}, {}, {36, 40}, {31, 33}, {34, 36}, {}, {}, {22, 25}, {}, {}, {}, {}, {28, 30}, {23, 24}, {}, {}, {19, 21}, {37, 38}, {37, 39}, {}, {24, 27}, {19, 22}, {}, {36, 40}, {}, {}, {23, 26}, {}, {}, {}, {}, {35, 38}, {46, 48}, {}, {}, {}, {}, {}, {}, {17, 18}, {}, {}, {43, 45}, {}, {}, {}, {}, {}, {37, 41}, {43, 46}, {}, {35, 37}, {}, {44, 47}, {}, {}, {}, {}, {}, {}, {}, {36, 39}, {35, 35}, {}, {29, 31}, {38, 41}, {}, {41, 43}, {33, 36}, {}, {}, {34, 35}, {}, {17, 18}, {}, {}, {}, {}, {}, {}, {32, 33}, {}, {24, 25}, {}, {}, {30, 34}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {27, 30}, {}, {}, {}, {}, {33, 36}, {}, {}, {}, {}, {35, 37}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {36, 39}, {}, {19, 22}, {43, 45}, {}, {}, {27, 30}, {28, 32}, {37, 41}, {}, {27, 30}, {27, 28}, {}, {}, {}, {}, {}, {}, {}, {}, {20, 22}, {}, {}, {}, {}, {}, {}, {}, {}, {35, 38}, {37, 41}, {}, {}, {}, {}, {}, {27, 29}, {}, {}, {}, {}, {}, {23, 25}, {}, {}, {32, 35}, {}, {}, {}}},
    {{{}, {}, {38, 40}, {}, {}, {}, {}, {}, {36, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {31, 34}, {}, {19, 20}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {47, 49}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {46, 47}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {36, 38}, {}, {}, {}, {}, {}, {}, {}, {28, 30}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {59, 61}, {}, {46, 47}, {51, 53}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {32, 33}, {}, {}, {}, {}, {}, {}, {}, {}, {57, 60}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 55}, {}, {}, {}, {}, {}, {41, 42}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {45, 47}, {}, {}, {47, 49}, {}, {}, {}, {53, 56}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {41, 42}, {}, {}, {}, {}, {}, {}, {}, {}, {47, 49}, {41, 43}, {42, 44}, {}, {}, {}, {27, 30}, {34, 36}, {}, {}, {}, {53, 56}, {}, {}, {}, {}, {}, {}, {}, {40, 42}, {}, {39, 42}, {28, 29}, {}, {}, {}, {}, {47, 48}, {}, {}, {}, {42, 45}, {49, 52}, {}, {}, {}, {}, {}, {}, {57, 60}, {48, 51}, {56, 58}, {}, {}, {}, {}, {43, 46}, {}, {23, 24}, {}, {38, 40}, {}, {}, {}, {}, {}, {56, 59}, {49, 52}, {53, 54}, {}, {}, {32, 34}, {56, 60}, {}, {}, {}, {}, {}, {54, 55}, {}, {46, 48}, {41, 43}, {}, {}, {}, {}, {52, 55}, {46, 49}, {}, {45, 48}, {38, 41}, {29, 30}, {}, {30, 31}, {49, 52}, {}, {}, {}, {38, 40}, {42, 44}, {}, {27, 28}, {}, {42, 46}, {43, 45}, {}, {}, {}, {}, {69, 72}, {}, {52, 54}, {65, 67}, {}, {}, {}, {}, {46, 48}, {52, 55}, {}, {}, {}, {}, {37, 39}, {}, {}, {}, {}, {}, {}, {}, {}, {72, 74}, {}, {}, {}, {}, {}, {}, {49, 52}, {}, {}, {52, 54}, {}, {}, {}, {}, {}, {42, 46}, {38, 41}, {}, {}, {}, {}, {}, {}, {}, {}, {64, 66}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {47, 49}, {55, 59}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {40, 42}, {32, 34}, {}, {}, {44, 46}, {52, 56}, {57, 59}, {}}},
    {{{}, {}, {48, 51}, {}, {32, 34}, {50, 50}, {}, {46, 48}, {}, {57, 59}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {32, 35}, {}, {}, {43, 44}, {}, {}, {}, {36, 38}, {}, {}, {}, {}, {}, {44, 45}, {45, 48}, {}, {}, {43, 45}, {}, {37, 39}, {}, {}, {}, {57, 60}, {41, 43}, {44, 45}, {}, {}, {38, 40}, {}, {}, {}, {35, 36}, {43, 46}, {30, 31}, {}, {}, {27, 28}, {46, 47}, {49, 52}, {}, {39, 40}, {32, 35}, {}, {53, 55}, {}, {}, {40, 43}, {}, {}, {}, {}, {52, 55}, {59, 61}, {}, {}, {}, {}, {}, {}, {25, 26}, {}, {}, {56, 58}, {}, {}, {}, {}, {}, {53, 56}, {55, 58}, {}, {49, 52}, {}, {61, 65}, {}, {}, {}, {}, {}, {}, {}, {52, 55}, {44, 47}, {}, {42, 45}, {56, 59}, {}, {56, 58}, {50, 52}, {}, {49, 51}, {42, 44}, {}, {26, 27}, {}, {}, {}, {}, {}, {}, {45, 47}, {}, {29, 31}, {}, {}, {46, 52}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {40, 42}, {}, {}, {}, {}, {48, 49}, {}, {}, {}, {}, {50, 52}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 56}, {}, {34, 36}, {55, 57}, {}, {}, {45, 48}, {}, {42, 45}, {}, {42, 45}, {38, 39}, {}, {}, {}, {}, {}, {}, {}, {}, {33, 35}, {}, {}, {}, {}, {}, {}, {}, {}, {50, 52}, {60, 64}, {}, {}, {}, {}, {}, {37, 40}, {}, {}, {}, {}, {}, {35, 37}, {}, {}, {47, 50}, {}, {}, {}}},
    {{{}, {}, {52, 55}, {}, {}, {}, {}, {}, {49, 51}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {47, 50}, {}, {27, 28}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {59, 60}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {56, 57}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {52, 55}, {}, {}, {}, {}, {}, {}, {}, {38, 40}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {73, 75}, {}, {55, 56}, {68, 69}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {42, 44}, {}, {}, {}, {}, {}, {}, {}, {}, {75, 78}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {67, 69}, {}, {}, {}, {}, {}, {50, 53}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {80, 83}, {85, 87}, {48, 50}, {73, 75}, {}, {72, 75}, {71, 76}, {}, {70, 73}, {70, 73}, {}, {}, {}, {}, {88, 91}, {}, {}, {}, {}, {}, {64, 66}, {}, {}, {}, {}, {}, {}, {}, {}, {72, 76}, {}, {}, {30, 33}, {}, {79, 81}, {49, 52}, {}, {}, {}, {}, {}, {}, {}, {39, 42}, {32, 33}, {63, 64}, {}, {}, {}, {}, {68, 70}, {57, 59}, {}, {}, {}, {}, {64, 66}, {78, 79}, {62, 65}, {56, 59}, {64, 66}, {}, {}, {93, 95}, {56, 61}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {104, 108}, {}, {37, 38}, {62, 64}, {50, 53}, {}, {}, {}, {}, {}, {82, 86}, {}, {80, 82}, {77, 78}, {73, 75}, {50, 51}, {}, {}, {}, {}, {64, 68}, {}, {}, {}, {88, 89}, {}, {}, {68, 71}, {88, 90}, {}, {}, {}, {36, 38}, {}, {}, {50, 51}, {68, 71}, {}, {}, {}, {}, {}, {}, {76, 78}, {52, 57}, {42, 44}, {72, 74}, {66, 69}, {88, 93}, {}, {}, {}, {94, 98}, {}, {}, {}, {}, {}, {}, {}, {}, {66, 69}, {}, {64, 65}, {}, {}, {}, {}, {74, 77}, {}, {}, {}, {}, {75, 78}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {74, 77}, {66, 68}, {55, 58}, {84, 86}, {}, {}, {55, 57}, {59, 62}, {67, 68}, {65, 68}, {61, 65}, {70, 74}, {}, {}, {}, {}, {}, {}, {95, 98}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {81, 85}, {}, {92, 94}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {68, 70}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {84, 89}, {}, {}, {80, 83}, {}, {}, {}, {}, {}, {}, {}, {}, {68, 72}, {}, {}, {}, {}, {}, {}, {}, {75, 79}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {77, 83}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {43, 46}, {34, 37}, {65, 69}, {75, 77}, {73, 78}, {74, 80}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {67, 72}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {54, 59}, {}, {}, {}, {77, 81}, {}, {}, {}, {83, 86}, {}, {}, {52, 54}, {}, {}, {}, {53, 57}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {71, 74}, {}, {}, {}, {}, {}, {}, {}, {83, 85}, {70, 76}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {78, 83}, {}, {}, {97, 99}, {}, {}, {}, {}, {}, {69, 75}, {}, {81, 83}, {}, {}, {}, {}, {53, 58}, {}, {}, {}, {}, {}, {}, {}, {}, {73, 75}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {71, 73}, {}, {}, {}, {90, 95}, {}, {}, {}}},
    {{{}, {}, {90, 95}, {}, {}, {84, 87}, {}, {}, {}, {}, {}, {}, {}, {}, {73, 78}, {}, {}, {}, {}, {}, {}, {}, {80, 84}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {84, 88}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {47, 50}, {38, 41}, {70, 74}, {78, 80}, {79, 83}, {81, 85}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {73, 78}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {60, 65}, {}, {}, {}, {82, 87}, {}, {}, {}, {87, 92}, {}, {}, {55, 56}, {}, {}, {}, {58, 61}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {75, 79}, {}, {}, {}, {}, {}, {}, {}, {86, 88}, {77, 81}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {84, 89}, {}, {}, {100, 102}, {}, {}, {}, {}, {}, {76, 80}, {}, {84, 86}, {}, {}, {}, {}, {59, 63}, {}, {}, {}, {}, {}, {}, {}, {}, {76, 78}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {74, 78}, {}, {}, {}, {96, 100}, {}, {}, {}}},
    {{{}, {}, {96, 101}, {}, {}, {88, 91}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 82}, {}, {}, {}, {}, {}, {}, {}, {85, 87}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {89, 94}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {51, 53}, {42, 46}, {75, 80}, {81, 83}, {84, 88}, {86, 89}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 84}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {66, 70}, {}, {}, {}, {88, 92}, {}, {}, {}, {93, 97}, {}, {}, {57, 59}, {}, {}, {}, {62, 66}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {80, 82}, {}, {}, {}, {}, {}, {}, {}, {89, 91}, {82, 89}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {90, 95}, {}, {}, {103, 105}, {}, {}, {}, {}, {}, {81, 88}, {}, {87, 89}, {}, {}, {}, {}, {64, 68}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 81}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 82}, {}, {}, {}, {101, 107}, {}, {}, {}}},
    {{{}, {}, {102, 105}, {}, {}, {92, 94}, {}, {}, {}, {}, {}, {}, {}, {}, {83, 86}, {}, {}, {}, {}, {}, {}, {}, {88, 90}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {95, 100}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {54, 57}, {47, 49}, {81, 85}, {84, 86}, {89, 95}, {90, 94}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {85, 88}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {71, 74}, {}, {}, {}, {93, 99}, {}, {}, {}, {98, 103}, {}, {}, {60, 60}, {}, {}, {}, {67, 71}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {83, 86}, {}, {}, {}, {}, {}, {}, {}, {92, 94}, {90, 93}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {96, 100}, {}, {}, {106, 108}, {}, {}, {}, {}, {}, {89, 92}, {}, {90, 93}, {}, {}, {}, {}, {69, 75}, {}, {}, {}, {}, {}, {}, {}, {}, {82, 84}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {83, 89}, {}, {}, {}, {108, 113}, {}, {}, {}}},
    {{{}, {}, {106, 110}, {}, {}, {95, 96}, {}, {}, {}, {}, {}, {}, {}, {}, {87, 90}, {}, {}, {}, {}, {}, {}, {}, {91, 93}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {101, 104}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {58, 60}, {50, 53}, {86, 91}, {87, 89}, {}, {95, 97}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {89, 94}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {75, 78}, {}, {}, {}, {100, 103}, {}, {}, {}, {104, 107}, {}, {}, {61, 62}, {}, {}, {}, {72, 75}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {45, 48}, {}, {75, 77}, {}, {79, 82}, {49, 52}, {87, 91}, {57, 60}, {}, {}, {83, 86}, {}, {}, {52, 55}, {95, 97}, {94, 98}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {65, 68}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {101, 105}, {}, {}, {109, 111}, {}, {}, {}, {}, {}, {93, 97}, {}, {94, 97}, {}, {}, {}, {}, {76, 80}, {}, {}, {}, {}, {}, {}, {}, {}, {85, 86}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {90, 93}, {}, {}, {}, {114, 118}, {}, {}, {}}},
    {{{}, {}, {111, 114}, {}, {}, {97, 99}, {}, {}, {}, {}, {}, {}, {}, {}, {91, 95}, {}, {}, {}, {}, {}, {}, {}, {94, 96}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {105, 108}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {61, 63}, {54, 56}, {92, 96}, {90, 92}, {96, 99}, {98, 100}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {95, 99}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 82}, {}, {}, {}, {104, 109}, {}, {}, {}, {108, 111}, {}, {}, {}, {}, {}, {}, {76, 80}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {92, 95}, {}, {}, {}, {}, {}, {}, {}, {98, 100}, {99, 102}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {106, 111}, {}, {}, {112, 114}, {}, {}, {}, {}, {}, {98, 101}, {}, {98, 101}, {}, {}, {}, {}, {81, 86}, {}, {}, {}, {}, {}, {}, {}, {}, {87, 92}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {94, 98}, {}, {}, {}, {119, 124}, {}, {}, {}}},
    {{{}, {}, {115, 117}, {}, {}, {100, 102}, {}, {}, {}, {}, {}, {}, {}, {}, {96, 97}, {}, {}, {}, {}, {}, {}, {}, {97, 98}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {109, 112}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {64, 64}, {57, 60}, {97, 101}, {93, 94}, {100, 101}, {101, 103}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {100, 101}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {83, 85}, {}, {}, {}, {110, 114}, {}, {}, {}, {112, 113}, {}, {}, {}, {}, {}, {}, {81, 84}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {96, 98}, {}, {}, {}, {}, {}, {}, {}, {101, 102}, {103, 104}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {112, 112}, {}, {}, {115, 116}, {}, {}, {}, {}, {}, {102, 106}, {}, {102, 103}, {}, {}, {}, {}, {87, 89}, {}, {}, {}, {}, {}, {}, {}, {}, {93, 94}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {99, 103}, {}, {}, {}, {125, 130}, {}, {}, {}}},
    {{{}, {}, {118, 125}, {105, 110}, {58, 60}, {103, 109}, {5, 9}, {}, {77, 82}, {108, 113}, {}, {}, {}, {44, 49}, {98, 102}, {34, 39}, {101, 106}, {77, 82}, {66, 73}, {65, 70}, {}, {}, {99, 103}, {}, {14, 20}, {}, {74, 77}, {}, {26, 30}, {15, 19}, {15, 19}, {77, 81}, {}, {}, {40, 50}, {43, 47}, {89, 96}, {59, 63}, {70, 76}, {66, 74}, {}, {69, 76}, {92, 99}, {69, 73}, {65, 70}, {65, 71}, {61, 65}, {102, 108}, {95, 102}, {102, 109}, {104, 108}, {63, 69}, {76, 84}, {65, 71}, {54, 60}, {55, 58}, {}, {82, 86}, {67, 72}, {86, 93}, {73, 80}, {}, {102, 105}, {103, 108}, {90, 95}, {}, {72, 78}, {}, {}, {}, {85, 94}, {}, {107, 113}, {68, 71}, {70, 74}, {}, {}, {117, 122}, {}, {43, 46}, {77, 84}, {86, 89}, {108, 116}, {}, {}, {115, 122}, {63, 68}, {87, 96}, {100, 104}, {114, 121}, {}, {}, {63, 67}, {108, 113}, {}, {40, 45}, {85, 90}, {77, 81}, {81, 87}, {76, 79}, {}, {98, 102}, {86, 89}, {}, {}, {}, {}, {110, 117}, {99, 105}, {49, 52}, {105, 111}, {78, 81}, {59, 65}, {88, 96}, {57, 62}, {99, 105}, {68, 75}, {46, 52}, {63, 70}, {87, 94}, {87, 94}, {}, {56, 60}, {103, 108}, {105, 109}, {106, 111}, {}, {39, 45}, {70, 74}, {118, 125}, {98, 103}, {}, {}, {91, 97}, {84, 90}, {}, {103, 111}, {67, 75}, {70, 75}, {107, 111}, {}, {}, {46, 50}, {}, {73, 79}, {}, {}, {}, {65, 71}, {66, 71}, {}, {}, {9, 13}, {106, 111}, {76, 80}, {52, 56}, {5, 9}, {41, 47}, {}, {}, {113, 120}, {}, {59, 65}, {117, 121}, {}, {5, 9}, {58, 63}, {73, 77}, {}, {107, 111}, {66, 72}, {}, {14, 18}, {}, {}, {3, 5}, {90, 96}, {21, 26}, {}, {91, 99}, {61, 66}, {25, 29}, {}, {}, {45, 50}, {95, 102}, {61, 67}, {53, 60}, {21, 24}, {94, 98}, {123, 130}, {}, {}, {20, 22}, {22, 26}, {19, 21}, {58, 63}, {}, {23, 27}, {26, 30}, {15, 18}, {104, 109}, {}, {}, {23, 27}, {131, 136}, {104, 109}, {102, 109}, {}}},
    {{{}, {}, {159, 162}, {147, 150}, {65, 68}, {143, 145}, {}, {132, 134}, {116, 120}, {149, 151}, {98, 101}, {98, 101}, {73, 76}, {}, {128, 129}, {56, 59}, {146, 148}, {93, 95}, {107, 110}, {76, 79}, {54, 57}, {}, {138, 141}, {}, {}, {164, 166}, {}, {}, {}, {}, {}, {113, 116}, {144, 146}, {98, 99}, {68, 69}, {60, 61}, {130, 132}, {95, 97}, {105, 107}, {111, 114}, {58, 59}, {115, 118}, {139, 143}, {110, 115}, {90, 92}, {81, 84}, {76, 78}, {141, 144}, {145, 147}, {153, 156}, {130, 131}, {107, 108}, {120, 123}, {100, 101}, {90, 93}, {87, 89}, {85, 86}, {120, 122}, {}, {131, 134}, {89, 92}, {96, 98}, {131, 134}, {134, 137}, {130, 133}, {147, 148}, {84, 87}, {}, {183, 186}, {}, {132, 135}, {148, 152}, {147, 150}, {}, {}, {}, {}, {152, 154}, {}, {58, 59}, {114, 117}, {121, 123}, {148, 152}, {}, {}, {155, 158}, {}, {127, 131}, {139, 142}, {150, 153}, {122, 125}, {114, 117}, {80, 82}, {147, 150}, {}, {}, {116, 117}, {109, 110}, {100, 104}, {89, 90}, {146, 148}, {126, 129}, {111, 112}, {85, 88}, {107, 110}, {152, 155}, {182, 186}, {157, 160}, {137, 140}, {71, 73}, {147, 150}, {107, 110}, {86, 88}, {123, 125}, {85, 87}, {133, 136}, {106, 108}, {79, 80}, {90, 93}, {118, 120}, {}, {117, 119}, {79, 80}, {132, 134}, {139, 143}, {146, 150}, {}, {}, {}, {171, 174}, {112, 114}, {61, 64}, {88, 90}, {107, 109}, {105, 108}, {168, 173}, {149, 151}, {}, {106, 109}, {137, 140}, {107, 109}, {}, {}, {165, 167}, {98, 100}, {122, 125}, {}, {157, 159}, {98, 100}, {105, 108}, {125, 127}, {}, {}, {121, 124}, {93, 95}, {84, 86}, {}, {}, {136, 138}, {}, {152, 155}, {109, 111}, {72, 75}, {161, 163}, {}, {}, {}, {84, 85}, {103, 107}, {141, 145}, {82, 83}, {135, 137}, {}, {}, {}, {}, {125, 127}, {}, {149, 152}, {109, 113}, {72, 74}, {}, {49, 52}, {}, {59, 62}, {135, 137}, {}, {}, {}, {133, 135}, {163, 168}, {152, 155}, {}, {}, {44, 47}, {33, 36}, {}, {}, {34, 37}, {}, {}, {142, 145}, {}, {53, 56}, {34, 37}, {168, 172}, {141, 144}, {143, 148}, {}}},
    {{{}, {}, {163, 165}, {151, 154}, {69, 72}, {146, 149}, {}, {135, 138}, {121, 124}, {152, 156}, {102, 105}, {102, 105}, {77, 80}, {}, {130, 131}, {60, 63}, {149, 150}, {96, 98}, {111, 114}, {80, 83}, {58, 61}, {}, {142, 144}, {}, {}, {167, 171}, {}, {}, {}, {}, {}, {117, 120}, {147, 149}, {100, 102}, {70, 71}, {62, 64}, {133, 136}, {98, 101}, {108, 110}, {115, 118}, {60, 62}, {119, 121}, {144, 148}, {116, 120}, {93, 95}, {85, 88}, {79, 81}, {145, 148}, {148, 149}, {157, 160}, {132, 135}, {109, 110}, {124, 127}, {102, 104}, {94, 98}, {90, 93}, {87, 88}, {123, 125}, {}, {135, 137}, {93, 97}, {99, 101}, {135, 138}, {138, 141}, {134, 137}, {149, 152}, {88, 91}, {}, {187, 190}, {}, {136, 139}, {153, 156}, {151, 154}, {}, {}, {}, {}, {155, 158}, {}, {60, 61}, {118, 120}, {124, 127}, {153, 154}, {}, {}, {159, 162}, {}, {132, 135}, {143, 146}, {154, 156}, {126, 129}, {118, 120}, {83, 85}, {151, 155}, {}, {}, {118, 120}, {111, 113}, {105, 108}, {91, 92}, {149, 156}, {130, 133}, {113, 116}, {89, 93}, {111, 114}, {156, 159}, {187, 191}, {161, 164}, {141, 144}, {74, 76}, {151, 154}, {111, 113}, {89, 91}, {126, 127}, {88, 90}, {137, 139}, {109, 112}, {81, 81}, {94, 96}, {121, 124}, {114, 116}, {120, 124}, {81, 83}, {135, 137}, {144, 146}, {151, 155}, {}, {}, {}, {175, 178}, {115, 117}, {65, 67}, {}, {110, 112}, {109, 113}, {174, 179}, {152, 156}, {}, {110, 113}, {141, 144}, {110, 113}, {}, {}, {168, 172}, {101, 103}, {126, 130}, {}, {160, 162}, {101, 103}, {109, 111}, {128, 130}, {}, {}, {125, 127}, {96, 98}, {87, 88}, {}, {}, {139, 141}, {}, {156, 159}, {112, 113}, {76, 78}, {164, 166}, {}, {}, {}, {86, 90}, {108, 111}, {146, 148}, {84, 87}, {138, 140}, {}, {}, {}, {}, {128, 130}, {}, {153, 156}, {114, 118}, {75, 77}, {}, {53, 56}, {}, {63, 66}, {138, 140}, {}, {}, {}, {136, 138}, {169, 172}, {156, 159}, {}, {}, {48, 51}, {37, 40}, {}, {}, {38, 41}, {}, {}, {146, 149}, {}, {57, 60}, {38, 41}, {173, 176}, {145, 150}, {149, 153}, {}}},
    {{{}, {}, {166, 169}, {155, 158}, {73, 74}, {150, 152}, {}, {139, 142}, {125, 129}, {157, 160}, {106, 108}, {106, 108}, {81, 83}, {}, {132, 133}, {64, 66}, {151, 154}, {99, 101}, {115, 118}, {84, 87}, {62, 64}, {}, {145, 149}, {}, {}, {172, 175}, {}, {}, {}, {}, {}, {121, 125}, {150, 153}, {103, 104}, {72, 75}, {65, 67}, {137, 139}, {102, 104}, {111, 113}, {119, 121}, {63, 64}, {122, 124}, {149, 153}, {121, 125}, {96, 98}, {89, 91}, {82, 83}, {149, 151}, {150, 151}, {161, 163}, {136, 138}, {111, 113}, {128, 130}, {105, 106}, {99, 102}, {94, 96}, {89, 90}, {126, 128}, {}, {138, 139}, {98, 100}, {102, 104}, {139, 143}, {142, 146}, {138, 142}, {153, 157}, {92, 95}, {}, {191, 193}, {}, {140, 143}, {157, 159}, {155, 157}, {}, {}, {}, {}, {159, 161}, {}, {62, 63}, {121, 123}, {128, 130}, {155, 157}, {}, {}, {163, 165}, {}, {136, 138}, {147, 149}, {157, 159}, {130, 134}, {121, 122}, {86, 87}, {156, 159}, {}, {}, {121, 123}, {114, 116}, {109, 112}, {93, 94}, {157, 162}, {134, 137}, {117, 119}, {94, 97}, {115, 117}, {160, 164}, {192, 197}, {165, 168}, {145, 148}, {77, 78}, {155, 157}, {114, 117}, {92, 94}, {128, 129}, {91, 93}, {140, 142}, {113, 115}, {82, 84}, {97, 99}, {125, 127}, {117, 119}, {125, 127}, {84, 86}, {138, 140}, {147, 149}, {156, 160}, {}, {}, {}, {179, 181}, {118, 120}, {68, 69}, {91, 95}, {113, 115}, {114, 117}, {180, 184}, {157, 160}, {}, {114, 118}, {145, 149}, {114, 116}, {}, {}, {173, 177}, {104, 106}, {131, 135}, {}, {163, 165}, {104, 106}, {112, 114}, {131, 133}, {}, {}, {128, 130}, {99, 100}, {89, 91}, {}, {}, {142, 144}, {}, {160, 162}, {114, 117}, {79, 83}, {167, 169}, {}, {}, {}, {91, 94}, {112, 115}, {149, 151}, {88, 91}, {141, 143}, {}, {}, {}, {}, {131, 134}, {}, {157, 160}, {119, 122}, {78, 80}, {}, {57, 59}, {}, {67, 69}, {141, 143}, {}, {}, {}, {139, 143}, {173, 176}, {160, 164}, {}, {}, {52, 54}, {41, 43}, {}, {}, {42, 45}, {}, {}, {150, 152}, {}, {61, 63}, {42, 44}, {177, 180}, {151, 155}, {154, 158}, {}}},
    {{{}, {}, {170, 172}, {159, 161}, {75, 77}, {153, 154}, {}, {143, 145}, {130, 132}, {161, 164}, {109, 111}, {109, 111}, {84, 88}, {}, {134, 136}, {67, 69}, {155, 158}, {102, 105}, {119, 122}, {88, 91}, {65, 67}, {}, {150, 154}, {}, {}, {176, 179}, {}, {}, {}, {}, {}, {126, 129}, {154, 155}, {105, 108}, {76, 81}, {68, 69}, {140, 142}, {105, 107}, {114, 116}, {122, 126}, {65, 67}, {125, 127}, {154, 159}, {126, 128}, {99, 100}, {92, 94}, {84, 86}, {152, 154}, {152, 153}, {164, 166}, {139, 141}, {114, 116}, {131, 133}, {107, 109}, {103, 106}, {97, 98}, {91, 92}, {129, 132}, {}, {140, 141}, {101, 103}, {105, 107}, {144, 146}, {147, 149}, {143, 148}, {158, 160}, {96, 99}, {}, {194, 198}, {}, {144, 147}, {160, 162}, {158, 160}, {}, {}, {}, {}, {162, 164}, {}, {64, 65}, {124, 127}, {131, 133}, {158, 163}, {}, {}, {166, 169}, {}, {139, 142}, {150, 153}, {160, 163}, {135, 136}, {123, 126}, {88, 90}, {160, 162}, {}, {}, {124, 126}, {117, 119}, {113, 115}, {95, 97}, {163, 170}, {138, 141}, {120, 123}, {98, 100}, {118, 119}, {165, 168}, {198, 202}, {169, 171}, {149, 150}, {}, {158, 160}, {118, 119}, {95, 97}, {130, 131}, {94, 96}, {143, 148}, {116, 118}, {85, 87}, {100, 102}, {128, 130}, {120, 122}, {128, 130}, {87, 89}, {141, 143}, {150, 153}, {161, 165}, {}, {}, {}, {182, 184}, {121, 123}, {70, 71}, {96, 97}, {116, 119}, {118, 122}, {185, 189}, {161, 163}, {}, {119, 122}, {150, 152}, {117, 119}, {}, {}, {178, 179}, {107, 109}, {136, 141}, {}, {166, 168}, {107, 109}, {115, 116}, {134, 136}, {}, {}, {131, 133}, {101, 103}, {92, 93}, {}, {}, {145, 148}, {}, {163, 165}, {118, 121}, {84, 86}, {170, 172}, {}, {}, {}, {95, 98}, {116, 118}, {152, 155}, {92, 97}, {144, 146}, {}, {}, {}, {}, {135, 137}, {}, {161, 163}, {123, 126}, {81, 83}, {}, {60, 62}, {}, {70, 72}, {144, 146}, {}, {}, {}, {144, 147}, {177, 179}, {165, 169}, {}, {}, {55, 57}, {44, 46}, {}, {}, {46, 47}, {}, {}, {153, 158}, {}, {64, 65}, {45, 47}, {181, 185}, {156, 158}, {159, 161}, {}}},
    {{{}, {}, {173, 176}, {162, 164}, {78, 80}, {155, 156}, {}, {146, 148}, {133, 135}, {165, 167}, {112, 113}, {112, 113}, {89, 91}, {}, {137, 138}, {70, 72}, {159, 160}, {106, 108}, {123, 125}, {92, 94}, {68, 70}, {}, {155, 158}, {}, {}, {180, 182}, {}, {}, {}, {}, {}, {130, 133}, {156, 158}, {109, 111}, {82, 84}, {70, 72}, {143, 145}, {108, 109}, {117, 120}, {127, 129}, {68, 69}, {128, 130}, {160, 163}, {129, 134}, {101, 102}, {95, 97}, {87, 88}, {155, 157}, {154, 156}, {167, 170}, {142, 145}, {117, 119}, {134, 136}, {110, 111}, {107, 110}, {99, 100}, {93, 94}, {133, 134}, {}, {142, 144}, {104, 105}, {108, 110}, {147, 149}, {150, 152}, {149, 152}, {161, 163}, {100, 102}, {}, {199, 201}, {}, {148, 150}, {163, 166}, {161, 163}, {}, {}, {}, {}, {165, 167}, {}, {66, 68}, {128, 130}, {134, 136}, {164, 166}, {}, {}, {170, 172}, {}, {143, 145}, {154, 156}, {164, 164}, {137, 138}, {127, 129}, {91, 92}, {163, 165}, {}, {}, {127, 128}, {120, 121}, {116, 119}, {98, 99}, {171, 175}, {142, 143}, {124, 126}, {101, 103}, {120, 122}, {169, 171}, {203, 205}, {172, 174}, {151, 154}, {79, 81}, {161, 163}, {120, 122}, {98, 100}, {132, 134}, {97, 98}, {149, 151}, {119, 121}, {88, 90}, {103, 105}, {131, 133}, {123, 125}, {131, 133}, {90, 92}, {144, 146}, {154, 157}, {166, 169}, {}, {}, {}, {185, 187}, {124, 126}, {72, 74}, {98, 99}, {120, 120}, {123, 126}, {190, 194}, {164, 166}, {}, {123, 123}, {153, 156}, {120, 122}, {}, {}, {180, 182}, {110, 112}, {142, 144}, {}, {169, 171}, {110, 111}, {117, 119}, {137, 139}, {}, {}, {134, 136}, {104, 106}, {94, 97}, {}, {}, {149, 150}, {}, {166, 168}, {122, 125}, {87, 89}, {173, 175}, {}, {}, {}, {99, 102}, {}, {156, 159}, {98, 101}, {147, 149}, {}, {}, {}, {}, {138, 140}, {}, {164, 165}, {127, 130}, {84, 86}, {}, {63, 65}, {}, {73, 75}, {147, 149}, {}, {}, {}, {148, 151}, {180, 182}, {170, 173}, {}, {}, {58, 60}, {47, 49}, {}, {}, {48, 49}, {}, {}, {159, 161}, {}, {66, 68}, {48, 50}, {186, 189}, {159, 161}, {162, 164}, {}}},
    {{{}, {}, {177, 179}, {165, 167}, {81, 83}, {157, 159}, {}, {149, 151}, {136, 139}, {168, 170}, {114, 116}, {114, 116}, {92, 94}, {}, {139, 140}, {73, 75}, {161, 163}, {109, 111}, {126, 128}, {95, 98}, {71, 73}, {}, {159, 162}, {}, {}, {183, 186}, {}, {}, {}, {}, {}, {134, 137}, {159, 161}, {112, 114}, {85, 88}, {73, 75}, {146, 148}, {110, 111}, {121, 123}, {130, 134}, {70, 71}, {131, 133}, {164, 167}, {135, 140}, {103, 104}, {98, 101}, {89, 90}, {158, 160}, {157, 159}, {171, 173}, {146, 149}, {120, 122}, {137, 139}, {112, 114}, {111, 114}, {101, 103}, {95, 96}, {135, 136}, {}, {145, 147}, {106, 107}, {111, 113}, {150, 152}, {153, 155}, {153, 156}, {164, 167}, {103, 106}, {}, {202, 205}, {}, {151, 153}, {167, 169}, {164, 165}, {}, {}, {}, {}, {168, 170}, {}, {69, 71}, {131, 133}, {137, 139}, {167, 169}, {}, {}, {173, 175}, {}, {146, 148}, {157, 159}, {165, 166}, {139, 141}, {130, 132}, {93, 93}, {166, 168}, {}, {}, {129, 130}, {122, 123}, {120, 123}, {100, 101}, {176, 180}, {144, 145}, {127, 129}, {104, 106}, {123, 125}, {172, 174}, {206, 208}, {175, 177}, {155, 158}, {82, 84}, {164, 166}, {123, 125}, {101, 103}, {135, 136}, {99, 101}, {152, 156}, {122, 124}, {91, 92}, {106, 108}, {134, 136}, {126, 128}, {134, 136}, {93, 94}, {147, 149}, {158, 161}, {170, 173}, {}, {}, {}, {188, 190}, {127, 129}, {75, 77}, {100, 101}, {121, 123}, {127, 129}, {195, 201}, {167, 170}, {}, {124, 124}, {157, 160}, {123, 123}, {}, {}, {183, 185}, {113, 115}, {145, 148}, {}, {172, 174}, {112, 114}, {120, 122}, {140, 142}, {}, {}, {137, 138}, {107, 108}, {98, 99}, {}, {}, {151, 152}, {}, {169, 171}, {126, 129}, {90, 92}, {176, 178}, {}, {}, {}, {103, 106}, {}, {160, 163}, {102, 105}, {150, 152}, {}, {}, {}, {}, {141, 143}, {}, {166, 168}, {131, 133}, {87, 88}, {}, {66, 68}, {}, {76, 78}, {150, 152}, {}, {}, {}, {152, 155}, {183, 185}, {174, 176}, {}, {}, {61, 63}, {50, 52}, {}, {}, {50, 52}, {}, {}, {162, 164}, {}, {69, 71}, {51, 53}, {190, 193}, {162, 165}, {165, 167}, {}}},
    {{{}, {}, {180, 181}, {168, 170}, {84, 86}, {160, 162}, {}, {152, 154}, {140, 142}, {171, 173}, {117, 118}, {117, 118}, {95, 98}, {}, {141, 142}, {76, 78}, {164, 165}, {112, 114}, {129, 131}, {99, 102}, {74, 76}, {}, {163, 165}, {}, {}, {187, 189}, {}, {}, {}, {}, {}, {138, 140}, {162, 164}, {115, 117}, {89, 90}, {76, 77}, {149, 150}, {112, 114}, {124, 126}, {135, 137}, {72, 73}, {134, 136}, {168, 171}, {141, 143}, {105, 106}, {102, 104}, {91, 93}, {161, 162}, {160, 161}, {174, 176}, {150, 152}, {123, 125}, {140, 142}, {115, 116}, {115, 117}, {104, 106}, {97, 99}, {137, 139}, {}, {148, 150}, {108, 110}, {114, 116}, {153, 155}, {156, 158}, {157, 160}, {168, 170}, {107, 110}, {}, {206, 209}, {}, {154, 157}, {170, 172}, {166, 168}, {}, {}, {}, {}, {171, 173}, {}, {72, 72}, {134, 136}, {140, 142}, {170, 171}, {}, {}, {176, 178}, {}, {149, 151}, {160, 162}, {167, 169}, {142, 144}, {133, 135}, {94, 95}, {169, 172}, {}, {}, {131, 132}, {124, 127}, {124, 126}, {102, 103}, {181, 183}, {146, 148}, {130, 132}, {107, 109}, {126, 128}, {175, 177}, {209, 211}, {178, 181}, {159, 162}, {85, 87}, {167, 169}, {126, 127}, {104, 106}, {137, 138}, {102, 103}, {157, 160}, {125, 127}, {93, 93}, {109, 110}, {137, 139}, {129, 132}, {137, 139}, {95, 96}, {150, 152}, {162, 164}, {174, 176}, {}, {}, {}, {191, 193}, {130, 132}, {78, 79}, {102, 103}, {124, 126}, {130, 134}, {202, 207}, {171, 174}, {}, {125, 127}, {161, 163}, {124, 125}, {}, {}, {186, 188}, {116, 117}, {149, 152}, {}, {175, 177}, {115, 115}, {123, 125}, {143, 145}, {}, {}, {139, 141}, {109, 111}, {100, 101}, {}, {}, {153, 154}, {}, {172, 174}, {130, 131}, {93, 96}, {179, 181}, {}, {}, {}, {107, 109}, {119, 123}, {164, 166}, {106, 109}, {153, 155}, {}, {}, {}, {}, {144, 146}, {}, {169, 171}, {134, 136}, {89, 90}, {}, {69, 71}, {}, {79, 81}, {153, 155}, {}, {}, {}, {156, 159}, {186, 188}, {177, 180}, {}, {}, {64, 66}, {53, 55}, {}, {}, {53, 54}, {}, {}, {165, 168}, {}, {72, 74}, {54, 56}, {194, 197}, {166, 171}, {168, 170}, {}}},
    {{{}, {}, {182, 185}, {171, 173}, {87, 89}, {163, 165}, {}, {155, 157}, {143, 145}, {174, 176}, {119, 121}, {119, 121}, {99, 101}, {}, {143, 144}, {79, 81}, {166, 169}, {115, 117}, {132, 135}, {103, 106}, {77, 79}, {}, {166, 169}, {}, {}, {190, 193}, {}, {}, {}, {}, {}, {141, 143}, {165, 166}, {118, 120}, {91, 95}, {78, 79}, {151, 153}, {115, 117}, {127, 128}, {138, 140}, {74, 75}, {137, 139}, {172, 177}, {144, 147}, {107, 109}, {105, 107}, {94, 96}, {163, 164}, {162, 164}, {177, 178}, {153, 155}, {126, 128}, {143, 145}, {117, 119}, {118, 120}, {107, 109}, {100, 100}, {140, 141}, {}, {151, 152}, {111, 113}, {117, 119}, {156, 158}, {159, 161}, {161, 164}, {171, 172}, {111, 114}, {}, {210, 213}, {}, {158, 161}, {173, 175}, {169, 171}, {}, {}, {}, {}, {174, 176}, {}, {73, 73}, {137, 139}, {143, 145}, {172, 174}, {}, {}, {179, 181}, {}, {152, 154}, {163, 165}, {170, 172}, {145, 147}, {136, 138}, {96, 98}, {173, 176}, {}, {}, {133, 134}, {128, 130}, {127, 130}, {104, 105}, {184, 186}, {149, 152}, {133, 135}, {110, 113}, {129, 132}, {178, 180}, {212, 216}, {182, 185}, {163, 166}, {88, 90}, {170, 172}, {128, 130}, {107, 109}, {139, 141}, {104, 105}, {161, 164}, {128, 130}, {94, 96}, {111, 113}, {140, 142}, {133, 135}, {140, 142}, {97, 98}, {153, 155}, {165, 167}, {177, 180}, {}, {}, {}, {194, 196}, {133, 135}, {80, 81}, {104, 106}, {127, 129}, {135, 138}, {208, 211}, {175, 177}, {}, {128, 130}, {164, 166}, {126, 127}, {}, {}, {189, 191}, {118, 119}, {153, 156}, {}, {178, 180}, {116, 117}, {126, 128}, {146, 148}, {}, {}, {142, 144}, {112, 114}, {102, 104}, {}, {}, {155, 156}, {}, {175, 177}, {132, 134}, {97, 99}, {182, 184}, {}, {}, {}, {110, 114}, {124, 126}, {167, 169}, {110, 113}, {156, 158}, {}, {}, {}, {}, {147, 149}, {}, {172, 174}, {137, 140}, {91, 92}, {}, {72, 74}, {}, {82, 84}, {156, 158}, {}, {}, {}, {160, 163}, {189, 192}, {181, 184}, {}, {}, {67, 69}, {56, 58}, {}, {}, {55, 57}, {}, {}, {169, 172}, {}, {75, 77}, {57, 59}, {198, 201}, {172, 175}, {171, 174}, {}}},
    {{{}, {}, {186, 187}, {174, 178}, {90, 93}, {166, 167}, {}, {158, 163}, {146, 149}, {177, 180}, {122, 124}, {122, 124}, {102, 104}, {}, {145, 146}, {82, 85}, {170, 173}, {118, 121}, {136, 140}, {107, 110}, {80, 83}, {}, {170, 174}, {}, {}, {194, 197}, {}, {}, {}, {}, {}, {144, 149}, {167, 171}, {121, 123}, {96, 98}, {80, 83}, {154, 156}, {118, 120}, {129, 131}, {141, 145}, {76, 78}, {140, 143}, {178, 181}, {148, 153}, {110, 112}, {108, 111}, {97, 98}, {165, 168}, {165, 167}, {179, 182}, {156, 158}, {129, 131}, {146, 150}, {120, 122}, {121, 126}, {110, 114}, {101, 103}, {142, 144}, {}, {153, 155}, {114, 117}, {120, 122}, {159, 161}, {162, 166}, {165, 167}, {173, 178}, {115, 118}, {}, {214, 217}, {}, {162, 164}, {176, 179}, {172, 175}, {}, {}, {}, {}, {177, 180}, {}, {74, 75}, {140, 142}, {146, 148}, {175, 178}, {}, {}, {182, 185}, {}, {155, 158}, {166, 171}, {}, {148, 151}, {139, 142}, {99, 100}, {177, 180}, {}, {}, {135, 135}, {131, 134}, {131, 136}, {106, 108}, {187, 192}, {153, 156}, {136, 139}, {114, 115}, {133, 136}, {181, 185}, {217, 221}, {186, 188}, {167, 171}, {91, 92}, {173, 175}, {131, 134}, {110, 110}, {142, 143}, {106, 107}, {165, 169}, {131, 132}, {97, 99}, {114, 116}, {143, 144}, {136, 137}, {143, 147}, {99, 99}, {156, 158}, {168, 169}, {181, 184}, {}, {}, {}, {197, 201}, {136, 138}, {82, 83}, {107, 109}, {130, 131}, {139, 143}, {212, 215}, {178, 181}, {}, {131, 134}, {167, 171}, {128, 131}, {}, {}, {192, 195}, {120, 122}, {157, 161}, {}, {181, 183}, {118, 121}, {129, 131}, {149, 151}, {}, {}, {145, 147}, {115, 117}, {105, 108}, {}, {}, {157, 160}, {}, {178, 181}, {135, 140}, {100, 103}, {185, 187}, {}, {}, {}, {115, 117}, {127, 131}, {170, 171}, {114, 118}, {159, 161}, {}, {}, {}, {}, {150, 153}, {}, {175, 178}, {141, 145}, {93, 95}, {}, {75, 78}, {}, {85, 88}, {159, 161}, {}, {}, {}, {164, 168}, {193, 198}, {185, 187}, {}, {}, {70, 73}, {59, 61}, {}, {}, {58, 58}, {}, {}, {173, 176}, {}, {78, 78}, {60, 63}, {202, 205}, {176, 178}, {175, 178}, {}}},
    {{{}, {}, {188, 190}, {179, 181}, {94, 97}, {168, 169}, {}, {164, 168}, {150, 153}, {181, 182}, {125, 126}, {125, 126}, {105, 107}, {}, {147, 149}, {86, 88}, {174, 176}, {122, 124}, {141, 144}, {111, 114}, {84, 87}, {}, {175, 178}, {}, {}, {198, 201}, {}, {}, {}, {}, {}, {150, 152}, {172, 173}, {124, 126}, {99, 101}, {84, 85}, {157, 158}, {121, 122}, {132, 134}, {146, 150}, {79, 80}, {144, 147}, {182, 186}, {154, 156}, {113, 114}, {112, 115}, {99, 100}, {169, 171}, {168, 169}, {183, 184}, {159, 161}, {132, 134}, {151, 154}, {123, 125}, {127, 129}, {115, 117}, {104, 105}, {145, 146}, {}, {156, 156}, {118, 121}, {123, 126}, {162, 164}, {167, 170}, {168, 171}, {179, 182}, {119, 122}, {}, {218, 220}, {}, {165, 167}, {180, 183}, {176, 179}, {}, {}, {}, {}, {181, 184}, {}, {76, 76}, {143, 145}, {149, 152}, {179, 179}, {}, {}, {186, 188}, {}, {159, 162}, {172, 176}, {173, 176}, {152, 153}, {143, 145}, {101, 101}, {181, 184}, {}, {}, {136, 138}, {135, 137}, {137, 140}, {109, 110}, {193, 194}, {157, 159}, {140, 142}, {116, 119}, {137, 139}, {186, 188}, {222, 225}, {189, 191}, {172, 174}, {93, 94}, {176, 178}, {135, 137}, {111, 113}, {144, 147}, {108, 108}, {170, 173}, {133, 134}, {100, 102}, {117, 119}, {145, 147}, {138, 140}, {148, 150}, {100, 101}, {159, 161}, {170, 173}, {185, 188}, {}, {}, {}, {202, 205}, {139, 141}, {84, 85}, {110, 112}, {132, 133}, {144, 146}, {216, 220}, {182, 185}, {}, {135, 138}, {172, 175}, {132, 134}, {}, {}, {196, 199}, {123, 125}, {162, 164}, {}, {184, 185}, {122, 123}, {132, 134}, {152, 154}, {}, {}, {148, 151}, {118, 120}, {109, 111}, {}, {}, {161, 163}, {}, {182, 185}, {141, 142}, {104, 105}, {188, 190}, {}, {}, {}, {118, 120}, {}, {172, 175}, {119, 121}, {162, 164}, {}, {}, {}, {}, {154, 155}, {}, {179, 182}, {146, 149}, {96, 97}, {}, {79, 80}, {}, {89, 92}, {162, 164}, {}, {}, {}, {169, 171}, {199, 203}, {188, 191}, {}, {}, {74, 76}, {62, 64}, {}, {}, {59, 61}, {}, {}, {177, 181}, {}, {79, 82}, {64, 67}, {206, 209}, {179, 181}, {179, 183}, {}}},
    {{{}, {}, {191, 194}, {182, 184}, {98, 101}, {170, 172}, {}, {169, 172}, {154, 158}, {183, 184}, {127, 129}, {127, 129}, {108, 110}, {}, {150, 151}, {89, 90}, {177, 180}, {125, 127}, {145, 148}, {115, 117}, {88, 90}, {}, {179, 182}, {}, {}, {202, 205}, {}, {}, {}, {}, {}, {153, 156}, {174, 175}, {127, 129}, {102, 103}, {86, 88}, {159, 161}, {123, 124}, {135, 137}, {151, 154}, {81, 82}, {148, 151}, {187, 190}, {157, 158}, {115, 117}, {116, 119}, {101, 102}, {172, 173}, {170, 171}, {185, 188}, {162, 164}, {135, 138}, {155, 157}, {126, 129}, {130, 132}, {118, 120}, {106, 106}, {147, 149}, {}, {157, 158}, {122, 125}, {127, 130}, {165, 168}, {171, 174}, {172, 175}, {183, 186}, {123, 126}, {}, {221, 223}, {}, {168, 171}, {184, 186}, {180, 184}, {}, {}, {}, {}, {185, 187}, {}, {77, 78}, {146, 147}, {153, 156}, {180, 182}, {}, {}, {189, 192}, {}, {163, 166}, {177, 180}, {177, 178}, {154, 156}, {146, 148}, {102, 104}, {185, 187}, {}, {}, {139, 141}, {138, 141}, {141, 144}, {111, 113}, {195, 197}, {160, 162}, {143, 145}, {120, 122}, {140, 142}, {189, 192}, {226, 229}, {192, 195}, {175, 178}, {95, 97}, {179, 181}, {138, 139}, {114, 115}, {148, 150}, {109, 110}, {174, 176}, {135, 137}, {103, 105}, {120, 122}, {148, 150}, {}, {151, 153}, {102, 102}, {162, 164}, {174, 177}, {189, 192}, {}, {}, {}, {206, 210}, {142, 143}, {86, 88}, {113, 115}, {134, 135}, {147, 150}, {221, 224}, {186, 190}, {}, {139, 142}, {176, 179}, {135, 137}, {}, {}, {200, 203}, {126, 128}, {165, 168}, {}, {186, 187}, {124, 125}, {135, 137}, {155, 157}, {}, {}, {152, 154}, {121, 123}, {112, 114}, {}, {}, {164, 166}, {}, {186, 188}, {143, 144}, {106, 107}, {191, 193}, {}, {}, {}, {121, 124}, {}, {176, 179}, {122, 125}, {165, 167}, {}, {}, {}, {}, {156, 158}, {}, {183, 186}, {150, 153}, {98, 99}, {}, {81, 84}, {}, {93, 96}, {165, 167}, {}, {}, {}, {172, 174}, {}, {192, 195}, {}, {}, {77, 80}, {65, 67}, {}, {}, {62, 63}, {}, {}, {182, 184}, {}, {83, 86}, {68, 71}, {210, 213}, {182, 184}, {184, 188}, {}}},
    {{{}, {}, {195, 201}, {}, {}, {}, {10, 14}, {}, {159, 165}, {}, {}, {}, {}, {}, {152, 156}, {}, {}, {}, {149, 157}, {}, {}, {}, {}, {}, {25, 32}, {}, {}, {}, {}, {}, {}, {157, 162}, {}, {}, {104, 111}, {89, 94}, {}, {125, 128}, {138, 143}, {}, {}, {}, {}, {159, 164}, {118, 123}, {120, 124}, {103, 107}, {174, 179}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {92, 96}, {}, {}, {}, {169, 174}, {}, {176, 181}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 86}, {148, 154}, {157, 164}, {}, {5, 7}, {}, {193, 201}, {}, {167, 171}, {}, {179, 182}, {}, {}, {105, 107}, {}, {5, 9}, {}, {142, 147}, {}, {}, {}, {198, 209}, {163, 169}, {}, {}, {}, {}, {230, 237}, {}, {}, {98, 101}, {182, 187}, {140, 146}, {116, 122}, {151, 157}, {111, 115}, {177, 185}, {138, 145}, {106, 111}, {}, {151, 158}, {141, 147}, {}, {103, 109}, {}, {178, 183}, {}, {4, 8}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {108, 114}, {143, 148}, {}, {}, {}, {}, {}, {129, 135}, {}, {5, 9}, {}, {126, 132}, {138, 146}, {}, {4, 8}, {14, 18}, {}, {}, {115, 120}, {10, 14}, {}, {}, {}, {189, 195}, {}, {108, 114}, {}, {}, {10, 14}, {68, 74}, {}, {}, {180, 185}, {126, 132}, {}, {20, 24}, {}, {}, {8, 14}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {168, 175}, {}, {}, {43, 46}, {}, {}, {}, {}, {30, 33}, {81, 83}, {68, 71}, {68, 75}, {}, {}, {}, {21, 25}, {185, 190}, {58, 62}, {}, {}, {}, {}, {}, {40, 44}}},
    {{{}, {}, {206, 211}, {189, 195}, {}, {184, 185}, {}, {}, {174, 179}, {191, 197}, {}, {}, {}, {}, {163, 166}, {}, {188, 190}, {}, {166, 172}, {}, {}, {}, {189, 194}, {}, {}, {}, {}, {}, {}, {}, {}, {170, 174}, {182, 186}, {137, 139}, {122, 128}, {100, 101}, {168, 172}, {136, 139}, {150, 152}, {162, 165}, {88, 92}, {160, 164}, {199, 205}, {173, 176}, {129, 132}, {}, {}, {187, 190}, {}, {197, 203}, {172, 176}, {144, 148}, {166, 172}, {135, 138}, {140, 145}, {127, 130}, {114, 117}, {156, 160}, {}, {165, 169}, {}, {}, {182, 186}, {182, 188}, {189, 193}, {}, {}, {}, {}, {}, {}, {}, {}, {117, 121}, {103, 107}, {}, {}, {194, 197}, {}, {91, 91}, {159, 163}, {172, 175}, {194, 200}, {}, {}, {209, 215}, {}, {180, 185}, {188, 193}, {190, 192}, {163, 168}, {}, {112, 115}, {196, 200}, {}, {}, {154, 158}, {147, 150}, {}, {}, {218, 222}, {176, 179}, {152, 155}, {}, {}, {202, 206}, {246, 248}, {203, 208}, {}, {105, 108}, {192, 195}, {151, 153}, {127, 131}, {162, 163}, {118, 121}, {193, 196}, {150, 151}, {117, 120}, {127, 130}, {164, 166}, {155, 159}, {162, 166}, {}, {171, 174}, {191, 193}, {203, 206}, {}, {}, {103, 107}, {220, 227}, {}, {}, {}, {}, {}, {}, {198, 202}, {124, 129}, {156, 162}, {186, 189}, {}, {}, {}, {210, 213}, {}, {}, {}, {194, 197}, {}, {}, {164, 168}, {}, {}, {}, {}, {}, {}, {}, {170, 173}, {}, {203, 208}, {152, 158}, {}, {202, 207}, {}, {}, {}, {}, {142, 146}, {193, 195}, {}, {170, 173}, {}, {}, {}, {}, {}, {}, {195, 201}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {182, 187}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {198, 203}, {}, {}, {}, {220, 227}, {192, 195}, {197, 200}, {}}},
    {{{}, {}, {212, 217}, {196, 200}, {}, {186, 189}, {}, {180, 183}, {180, 182}, {198, 203}, {}, {}, {}, {}, {167, 169}, {}, {191, 195}, {}, {173, 180}, {}, {}, {}, {195, 200}, {}, {}, {}, {}, {}, {}, {}, {}, {175, 180}, {187, 189}, {}, {129, 131}, {102, 105}, {173, 177}, {140, 143}, {153, 156}, {166, 169}, {93, 97}, {165, 169}, {206, 210}, {177, 181}, {133, 135}, {}, {}, {191, 195}, {}, {204, 205}, {177, 181}, {149, 153}, {173, 177}, {139, 141}, {146, 151}, {131, 134}, {118, 121}, {161, 163}, {}, {170, 172}, {}, {}, {187, 192}, {189, 193}, {194, 199}, {}, {}, {}, {}, {}, {}, {}, {}, {122, 126}, {}, {}, {}, {198, 202}, {}, {92, 94}, {164, 167}, {176, 178}, {201, 206}, {}, {}, {216, 220}, {}, {186, 191}, {194, 198}, {193, 196}, {169, 173}, {}, {116, 118}, {201, 206}, {}, {}, {159, 162}, {151, 154}, {}, {}, {223, 228}, {180, 182}, {156, 159}, {}, {}, {207, 213}, {249, 252}, {209, 214}, {}, {109, 112}, {196, 200}, {154, 157}, {132, 136}, {164, 167}, {122, 125}, {197, 200}, {152, 155}, {121, 125}, {131, 133}, {167, 169}, {160, 163}, {167, 171}, {113, 116}, {175, 178}, {194, 198}, {207, 211}, {}, {}, {}, {228, 233}, {}, {}, {}, {}, {}, {}, {203, 207}, {130, 134}, {163, 166}, {190, 193}, {}, {}, {}, {214, 216}, {}, {}, {}, {198, 200}, {}, {}, {169, 172}, {}, {}, {}, {}, {}, {}, {}, {174, 177}, {}, {209, 213}, {159, 161}, {}, {208, 210}, {}, {}, {}, {}, {147, 152}, {196, 200}, {}, {174, 176}, {}, {}, {}, {}, {}, {}, {202, 205}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {188, 193}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {204, 208}, {}, {}, {}, {228, 232}, {196, 200}, {201, 204}, {}}},
    {{{}, {}, {218, 221}, {}, {}, {190, 190}, {}, {214, 216}, {188, 190}, {209, 210}, {}, {}, {}, {}, {170, 172}, {}, {}, {}, {}, {}, {}, {}, {201, 204}, {}, {}, {}, {123, 125}, {}, {}, {}, {}, {186, 189}, {194, 196}, {140, 142}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {132, 134}, {115, 118}, {}, {}, {}, {182, 184}, {154, 156}, {178, 182}, {142, 146}, {152, 157}, {}, {}, {164, 166}, {110, 113}, {}, {}, {163, 165}, {196, 199}, {194, 196}, {204, 208}, {198, 201}, {}, {}, {}, {}, {189, 193}, {195, 197}, {193, 196}, {127, 130}, {108, 110}, {}, {}, {203, 207}, {}, {95, 97}, {168, 171}, {179, 181}, {207, 209}, {}, {}, {221, 224}, {}, {196, 197}, {205, 208}, {197, 199}, {}, {}, {119, 120}, {207, 210}, {}, {}, {163, 166}, {157, 159}, {}, {}, {}, {183, 185}, {164, 165}, {}, {185, 187}, {}, {}, {220, 224}, {192, 194}, {113, 115}, {212, 214}, {158, 160}, {}, {168, 169}, {126, 128}, {}, {}, {}, {134, 134}, {170, 171}, {164, 165}, {208, 210}, {117, 120}, {179, 182}, {199, 201}, {218, 222}, {}, {}, {108, 110}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {167, 170}, {197, 199}, {}, {}, {}, {217, 218}, {148, 150}, {}, {}, {201, 203}, {}, {172, 174}, {204, 206}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {214, 216}, {}, {}, {211, 215}, {}, {}, {}, {}, {184, 186}, {205, 207}, {148, 151}, {177, 178}, {}, {}, {}, {}, {164, 166}, {}, {206, 209}, {}, {}, {}, {}, {}, {}, {184, 186}, {}, {}, {}, {194, 197}, {214, 217}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {209, 210}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {222, 222}, {206, 207}, {}, {191, 192}, {}, {217, 218}, {191, 193}, {211, 212}, {}, {}, {}, {}, {173, 174}, {}, {201, 202}, {}, {}, {}, {}, {}, {205, 206}, {}, {}, {219, 220}, {126, 127}, {}, {}, {}, {}, {190, 191}, {197, 198}, {143, 144}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {167, 167}, {114, 115}, {173, 175}, {}, {166, 167}, {200, 201}, {197, 198}, {209, 211}, {202, 203}, {}, {}, {230, 232}, {}, {194, 196}, {198, 199}, {197, 200}, {131, 132}, {111, 111}, {}, {}, {208, 209}, {}, {98, 98}, {172, 173}, {182, 182}, {210, 211}, {}, {}, {225, 226}, {}, {198, 199}, {209, 210}, {200, 202}, {176, 177}, {176, 177}, {}, {211, 213}, {}, {}, {167, 168}, {160, 161}, {}, {}, {229, 231}, {186, 186}, {166, 166}, {}, {188, 190}, {246, 248}, {253, 254}, {225, 227}, {195, 197}, {116, 117}, {215, 217}, {161, 162}, {137, 138}, {170, 170}, {129, 130}, {201, 202}, {156, 157}, {}, {135, 136}, {172, 173}, {166, 168}, {211, 214}, {121, 121}, {183, 184}, {202, 203}, {223, 226}, {}, {}, {111, 111}, {234, 235}, {}, {}, {}, {}, {}, {231, 233}, {}, {}, {171, 173}, {200, 202}, {}, {}, {}, {219, 220}, {151, 152}, {199, 199}, {}, {204, 205}, {150, 151}, {175, 176}, {207, 208}, {}, {}, {}, {}, {}, {}, {}, {178, 178}, {}, {217, 218}, {}, {}, {216, 217}, {}, {}, {}, {}, {187, 188}, {208, 209}, {152, 154}, {179, 181}, {}, {}, {}, {}, {167, 168}, {}, {210, 211}, {}, {}, {}, {}, {}, {}, {187, 188}, {}, {}, {}, {198, 199}, {218, 219}, {201, 202}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {211, 214}, {}, {}, {}, {233, 235}, {}, {}, {}}},
    {{{}, {}, {223, 225}, {}, {}, {193, 195}, {}, {219, 224}, {194, 200}, {213, 215}, {}, {}, {}, {}, {175, 180}, {}, {}, {}, {}, {}, {}, {}, {207, 212}, {}, {}, {}, {128, 131}, {}, {}, {}, {}, {192, 197}, {199, 201}, {145, 149}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {135, 138}, {119, 121}, {}, {}, {}, {185, 189}, {157, 160}, {183, 187}, {147, 150}, {158, 161}, {}, {}, {168, 172}, {116, 118}, {}, {}, {168, 171}, {202, 205}, {199, 201}, {212, 216}, {204, 207}, {}, {}, {}, {}, {197, 202}, {200, 204}, {201, 204}, {133, 136}, {112, 115}, {}, {}, {210, 215}, {}, {99, 101}, {174, 176}, {183, 185}, {212, 215}, {}, {}, {227, 230}, {}, {200, 205}, {211, 214}, {203, 206}, {}, {}, {121, 122}, {214, 220}, {}, {}, {169, 172}, {162, 164}, {}, {}, {}, {187, 190}, {167, 169}, {}, {191, 193}, {}, {}, {228, 232}, {198, 203}, {118, 120}, {218, 223}, {163, 166}, {}, {171, 173}, {131, 134}, {}, {}, {}, {137, 140}, {174, 177}, {169, 173}, {215, 218}, {122, 125}, {185, 187}, {204, 207}, {227, 231}, {}, {}, {112, 115}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {174, 179}, {203, 206}, {}, {}, {}, {221, 223}, {153, 157}, {}, {}, {206, 209}, {}, {177, 180}, {209, 214}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {219, 222}, {}, {}, {218, 222}, {}, {}, {}, {}, {189, 193}, {210, 213}, {155, 159}, {182, 187}, {}, {}, {}, {}, {169, 171}, {}, {212, 215}, {}, {}, {}, {}, {}, {}, {189, 192}, {}, {}, {}, {200, 204}, {220, 223}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {215, 218}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {226, 227}, {}, {}, {196, 198}, {}, {225, 227}, {201, 203}, {216, 218}, {}, {}, {}, {}, {181, 183}, {}, {}, {}, {}, {}, {}, {}, {213, 215}, {}, {}, {}, {132, 134}, {}, {}, {}, {}, {198, 201}, {202, 204}, {150, 151}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {139, 141}, {122, 125}, {}, {}, {}, {190, 192}, {161, 163}, {188, 191}, {151, 154}, {162, 166}, {}, {}, {173, 175}, {119, 121}, {}, {}, {172, 174}, {206, 208}, {202, 204}, {217, 220}, {208, 210}, {}, {}, {}, {}, {203, 207}, {205, 208}, {205, 208}, {137, 140}, {116, 118}, {}, {}, {216, 220}, {}, {102, 102}, {177, 179}, {186, 188}, {216, 218}, {}, {}, {231, 233}, {}, {206, 208}, {215, 218}, {207, 210}, {}, {}, {123, 124}, {221, 224}, {}, {}, {173, 175}, {165, 166}, {}, {}, {}, {191, 192}, {170, 172}, {}, {194, 197}, {}, {}, {233, 236}, {204, 207}, {121, 122}, {224, 225}, {167, 169}, {}, {174, 176}, {135, 136}, {}, {}, {}, {141, 142}, {178, 180}, {174, 176}, {219, 221}, {126, 127}, {188, 192}, {208, 210}, {232, 235}, {}, {}, {116, 118}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {180, 184}, {207, 210}, {}, {}, {}, {224, 225}, {158, 158}, {}, {}, {210, 212}, {}, {181, 183}, {215, 218}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {223, 225}, {}, {}, {223, 226}, {}, {}, {}, {}, {194, 196}, {214, 216}, {160, 162}, {188, 190}, {}, {}, {}, {}, {172, 174}, {}, {216, 218}, {}, {}, {}, {}, {}, {}, {193, 195}, {}, {}, {}, {205, 206}, {224, 226}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {219, 222}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {228, 231}, {}, {}, {199, 201}, {}, {228, 230}, {204, 206}, {219, 221}, {}, {}, {}, {}, {184, 188}, {}, {}, {}, {}, {}, {}, {}, {216, 220}, {}, {}, {}, {135, 138}, {}, {}, {}, {}, {202, 205}, {205, 208}, {152, 154}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {142, 145}, {126, 127}, {}, {}, {}, {193, 195}, {164, 167}, {192, 195}, {155, 158}, {167, 168}, {}, {}, {176, 179}, {122, 125}, {}, {}, {175, 177}, {209, 212}, {205, 208}, {221, 225}, {211, 215}, {}, {}, {}, {}, {208, 211}, {209, 212}, {209, 212}, {141, 144}, {119, 123}, {}, {}, {221, 226}, {}, {103, 106}, {180, 182}, {189, 191}, {219, 221}, {}, {}, {234, 237}, {}, {209, 212}, {219, 222}, {211, 213}, {}, {}, {125, 128}, {225, 227}, {}, {}, {176, 178}, {167, 170}, {}, {}, {}, {193, 195}, {173, 175}, {}, {198, 201}, {}, {}, {237, 241}, {208, 211}, {123, 124}, {226, 228}, {170, 172}, {}, {177, 178}, {137, 139}, {}, {}, {}, {143, 146}, {181, 182}, {177, 179}, {222, 225}, {128, 130}, {193, 196}, {}, {236, 239}, {}, {}, {119, 123}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {185, 188}, {211, 214}, {}, {}, {}, {226, 228}, {159, 160}, {}, {}, {213, 214}, {}, {184, 186}, {219, 222}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {226, 229}, {}, {}, {227, 230}, {}, {}, {}, {}, {197, 201}, {}, {163, 165}, {191, 194}, {}, {}, {}, {}, {175, 178}, {}, {219, 221}, {}, {}, {}, {}, {}, {}, {196, 199}, {}, {}, {}, {207, 209}, {227, 230}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {223, 227}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {232, 235}, {}, {}, {202, 204}, {}, {231, 232}, {207, 208}, {222, 223}, {}, {}, {}, {}, {189, 192}, {}, {}, {}, {}, {}, {}, {}, {221, 223}, {}, {}, {}, {139, 144}, {}, {}, {}, {}, {206, 208}, {209, 210}, {155, 157}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {146, 147}, {128, 128}, {}, {}, {}, {196, 197}, {168, 170}, {196, 197}, {159, 162}, {169, 171}, {}, {}, {180, 181}, {126, 129}, {}, {}, {178, 180}, {213, 216}, {209, 210}, {226, 228}, {216, 216}, {}, {}, {}, {}, {212, 213}, {213, 214}, {213, 215}, {145, 146}, {124, 126}, {}, {}, {227, 230}, {}, {}, {183, 185}, {192, 193}, {222, 223}, {}, {}, {238, 240}, {}, {213, 214}, {223, 225}, {214, 215}, {}, {}, {129, 130}, {228, 231}, {}, {}, {179, 180}, {171, 174}, {}, {}, {}, {196, 198}, {176, 177}, {}, {202, 202}, {}, {}, {242, 243}, {212, 214}, {125, 127}, {229, 230}, {173, 174}, {}, {179, 179}, {140, 141}, {}, {}, {}, {147, 148}, {183, 184}, {180, 182}, {226, 227}, {131, 131}, {197, 197}, {211, 214}, {240, 242}, {}, {}, {124, 126}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {189, 191}, {215, 216}, {}, {}, {}, {229, 231}, {161, 162}, {}, {}, {215, 217}, {}, {187, 188}, {223, 224}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {230, 231}, {}, {}, {231, 232}, {}, {}, {}, {}, {202, 204}, {217, 220}, {166, 167}, {195, 197}, {}, {}, {}, {}, {179, 181}, {}, {222, 224}, {}, {}, {}, {}, {}, {}, {200, 201}, {}, {}, {}, {210, 211}, {231, 232}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {228, 231}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {32, 35}, {19, 21}, {}, {}, {}, {}, {44, 47}, {}, {}, {}, {}, {}, {}, {32, 35}, {}, {}, {23, 25}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {36, 38}, {}, {}, {}, {33, 35}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {26, 29}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {29, 32}, {28, 31}, {}, {}, {41, 44}, {}, {32, 35}, {27, 30}, {}, {}, {}, {}, {39, 42}, {49, 51}, {}, {}, {}, {}, {}, {}, {}, {24, 26}, {}, {46, 48}, {}, {}, {}, {}, {}, {42, 44}, {}, {26, 28}, {}, {}, {48, 50}, {}, {}, {}, {22, 24}, {}, {}, {}, {}, {36, 37}, {}, {}, {}, {}, {44, 47}, {37, 40}, {21, 23}, {}, {}, {}, {19, 20}, {}, {}, {}, {}, {}, {}, {34, 36}, {}, {}, {32, 34}, {}, {35, 38}, {}, {}, {}, {35, 36}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {42, 45}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {24, 27}, {23, 26}, {}, {}, {}, {}, {33, 35}, {}, {}, {31, 33}, {29, 31}, {}, {}, {}, {}, {}, {}, {32, 35}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {39, 42}, {42, 45}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {26, 28}, {}, {}, {36, 39}, {}, {}, {}}},
    {{{}, {}, {}, {36, 37}, {}, {}, {}, {35, 37}, {}, {}, {22, 24}, {22, 24}, {19, 20}, {}, {}, {}, {36, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {36, 39}, {}, {26, 29}, {}, {}, {}, {}, {}, {}, {}, {}, {30, 33}, {}, {}, {}, {}, {31, 33}, {25, 25}, {}, {}, {}, {}, {}, {33, 35}, {}, {23, 25}, {}, {}, {}, {36, 39}, {}, {}, {49, 52}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {27, 28}, {}, {49, 51}, {}, {}, {}, {}, {}, {}, {}, {29, 32}, {38, 40}, {}, {}, {}, {}, {}, {}, {}, {}, {34, 37}, {}, {}, {}, {32, 34}, {42, 46}, {49, 51}, {}, {}, {}, {}, {}, {}, {21, 22}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {35, 37}, {}, {}, {}, {}, {}, {37, 38}, {}, {}, {}, {}, {}, {41, 44}, {40, 42}, {}, {}, {}, {31, 34}, {}, {}, {47, 49}, {}, {37, 39}, {}, {44, 46}, {}, {}, {38, 40}, {}, {}, {}, {}, {}, {}, {}, {42, 43}, {}, {}, {28, 29}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {32, 34}, {}, {}, {}, {}, {}, {}, {36, 39}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {33, 35}, {}, {}, {}, {}, {30, 32}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {38, 40}, {22, 23}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {39, 42}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {33, 36}, {}, {}, {}, {}, {}, {19, 23}, {17, 19}, {}, {27, 29}, {31, 34}, {}, {28, 29}, {}, {}, {}, {}, {}, {}, {}, {36, 37}, {}, {}, {}, {}, {46, 48}, {40, 43}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {33, 35}, {}, {}, {}, {}, {}, {}, {25, 27}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {41, 44}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {39, 43}, {}, {}, {}, {}, {}, {}, {43, 46}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {30, 31}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {40, 43}, {}, {23, 25}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {45, 47}, {42, 46}, {}}},
    {{{}, {}, {}, {47, 50}, {35, 36}, {}, {}, {}, {}, {60, 63}, {}, {}, {}, {}, {}, {}, {48, 49}, {}, {}, {36, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {46, 48}, {}, {}, {}, {46, 48}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {41, 43}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {40, 42}, {41, 43}, {}, {}, {56, 59}, {}, {48, 50}, {44, 47}, {}, {}, {}, {}, {56, 59}, {62, 64}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {59, 62}, {}, {}, {}, {}, {}, {57, 60}, {}, {41, 43}, {}, {}, {66, 69}, {}, {}, {}, {33, 36}, {}, {}, {}, {}, {48, 50}, {}, {}, {}, {}, {59, 61}, {53, 56}, {}, {}, {}, {}, {28, 30}, {}, {}, {}, {}, {}, {}, {48, 48}, {}, {}, {38, 40}, {}, {53, 56}, {}, {}, {}, {48, 50}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {56, 59}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {36, 38}, {37, 40}, {}, {}, {}, {}, {}, {}, {}, {46, 48}, {40, 42}, {}, {}, {}, {}, {}, {}, {49, 52}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 56}, {65, 69}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {38, 39}, {}, {}, {51, 54}, {}, {}, {}}},
    {{{}, {}, {}, {51, 52}, {}, {}, {}, {49, 52}, {}, {}, {27, 29}, {27, 29}, {24, 25}, {}, {}, {}, {50, 52}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {49, 52}, {}, {40, 42}, {}, {}, {}, {}, {}, {}, {}, {}, {44, 47}, {}, {}, {}, {}, {47, 50}, {32, 32}, {}, {}, {}, {}, {}, {43, 45}, {}, {36, 38}, {}, {}, {}, {51, 54}, {}, {}, {60, 63}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {33, 35}, {}, {63, 66}, {}, {}, {}, {}, {}, {}, {}, {44, 47}, {53, 55}, {}, {}, {}, {}, {}, {}, {}, {}, {42, 45}, {}, {}, {}, {46, 48}, {60, 64}, {60, 63}, {}, {}, {}, {}, {}, {}, {31, 33}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {41, 43}, {}, {}, {}, {}, {}, {51, 54}, {}, {}, {}, {}, {}, {50, 53}, {52, 55}, {}, {}, {}, {43, 46}, {}, {}, {58, 60}, {}, {50, 54}, {}, {53, 55}, {}, {}, {53, 55}, {}, {}, {}, {}, {}, {}, {}, {50, 52}, {}, {}, {39, 40}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {43, 45}, {}, {}, {}, {}, {}, {}, {53, 56}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {44, 47}, {}, {}, {}, {}, {41, 44}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {53, 54}, {37, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 55}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {37, 40}, {}, {}, {}, {}, {}, {27, 30}, {23, 25}, {}, {34, 36}, {39, 41}, {}, {37, 39}, {}, {}, {}, {}, {}, {}, {}, {46, 47}, {}, {}, {}, {}, {61, 64}, {55, 57}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {48, 50}, {}, {}, {}, {}, {}, {}, {37, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 55}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {55, 58}, {}, {}, {}, {}, {}, {}, {56, 59}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {41, 42}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {57, 60}, {}, {36, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {57, 60}, {60, 64}, {}}},
    {{{}, {}, {}, {55, 56}, {}, {}, {}, {}, {}, {64, 68}, {30, 36}, {30, 36}, {26, 32}, {}, {}, {10, 13}, {56, 59}, {}, {}, {}, {13, 16}, {}, {}, {}, {}, {58, 61}, {39, 41}, {}, {11, 16}, {}, {}, {}, {49, 51}, {}, {}, {}, {53, 59}, {31, 34}, {43, 49}, {}, {}, {42, 48}, {}, {}, {}, {}, {}, {48, 50}, {37, 45}, {42, 49}, {43, 48}, {40, 45}, {51, 57}, {33, 38}, {30, 34}, {31, 35}, {29, 33}, {48, 54}, {}, {48, 51}, {}, {}, {}, {60, 63}, {}, {58, 61}, {}, {}, {64, 68}, {}, {}, {60, 64}, {65, 69}, {37, 42}, {36, 40}, {}, {}, {51, 57}, {}, {}, {36, 39}, {}, {67, 72}, {}, {}, {}, {28, 32}, {}, {61, 63}, {}, {51, 52}, {}, {}, {70, 72}, {}, {}, {}, {39, 41}, {}, {}, {46, 52}, {}, {51, 54}, {}, {}, {}, {64, 72}, {62, 65}, {57, 60}, {}, {56, 62}, {}, {}, {}, {}, {}, {28, 31}, {}, {}, {}, {}, {}, {}, {}, {}, {57, 60}, {}, {}, {36, 40}, {59, 66}, {}, {}, {}, {}, {}, {54, 62}, {60, 65}, {32, 35}, {}, {60, 63}, {}, {}, {}, {61, 70}, {}, {}, {}, {56, 63}, {37, 42}, {38, 44}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 59}, {}, {}, {43, 45}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {46, 49}, {}, {}, {}, {}, {}, {}, {61, 64}, {}, {}, {10, 14}, {8, 11}, {}, {}, {}, {}, {}, {}, {57, 59}, {70, 72}, {48, 53}, {}, {}, {}, {}, {}, {}, {}, {11, 16}, {}, {}, {}, {}, {}, {55, 59}, {61, 69}, {65, 71}, {}}},
    {{{}, {}, {}, {79, 84}, {}, {}, {}, {66, 71}, {}, {88, 93}, {56, 59}, {56, 59}, {45, 49}, {}, {}, {}, {83, 87}, {}, {}, {}, {}, {}, {}, {}, {}, {96, 101}, {58, 61}, {}, {}, {}, {}, {}, {65, 68}, {64, 66}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {75, 77}, {}, {50, 55}, {}, {81, 87}, {}, {85, 92}, {}, {}, {103, 110}, {}, {}, {82, 88}, {88, 91}, {59, 61}, {58, 61}, {}, {}, {95, 97}, {}, {}, {57, 61}, {}, {92, 97}, {}, {}, {}, {49, 52}, {}, {79, 85}, {}, {70, 76}, {67, 72}, {}, {91, 98}, {}, {}, {}, {61, 63}, {}, {}, {84, 86}, {}, {68, 73}, {}, {62, 67}, {82, 87}, {104, 111}, {84, 92}, {76, 82}, {}, {87, 93}, {}, {}, {62, 67}, {}, {}, {}, {}, {}, {70, 75}, {73, 75}, {46, 51}, {}, {68, 71}, {}, {78, 87}, {}, {}, {58, 61}, {87, 93}, {}, {}, {}, {}, {}, {100, 106}, {91, 95}, {61, 66}, {}, {83, 88}, {60, 63}, {}, {}, {106, 111}, {}, {69, 73}, {}, {100, 104}, {}, {}, {69, 74}, {}, {}, {}, {}, {}, {}, {}, {86, 92}, {}, {}, {60, 65}, {}, {}, {}, {}, {}, {}, {62, 66}, {}, {}, {67, 69}, {}, {}, {}, {}, {}, {}, {88, 94}, {}, {}, {}, {}, {}, {29, 32}, {}, {}, {}, {}, {75, 80}, {91, 98}, {83, 91}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {62, 67}, {}, {}, {}, {73, 78}, {}, {}, {}}},
    {{{}, {}, {}, {201, 205}, {108, 110}, {}, {}, {}, {183, 187}, {204, 208}, {}, {}, {}, {}, {}, {}, {196, 200}, {}, {181, 186}, {}, {}, {}, {}, {}, {}, {213, 218}, {115, 122}, {161, 163}, {}, {}, {}, {181, 185}, {190, 193}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {105, 109}, {}, {}, {}, {193, 195}, {}, {200, 203}, {195, 197}, {}, {}, {}, {}, {182, 188}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {192, 195}, {199, 204}, {}, {174, 175}, {}, {}, {}, {}, {}, {}, {155, 156}, {}, {}, {}, {}, {160, 163}, {}, {}, {241, 245}, {}, {215, 219}, {186, 191}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {212, 217}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {194, 196}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {121, 125}, {}, {}, {}, {82, 87}, {132, 134}, {}, {201, 204}, {140, 147}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {215, 217}, {}, {}, {}, {}, {217, 221}, {228, 231}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {232, 236}, {}, {}, {}, {151, 152}, {}, {}, {}, {}, {216, 219}, {}, {158, 160}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {134, 137}, {}, {}, {}, {221, 224}, {}, {235, 238}, {}, {}, {}, {}, {}, {220, 225}, {221, 224}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {224, 228}, {232, 236}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {221, 225}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {197, 201}, {223, 226}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {227, 231}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {220, 223}, {239, 244}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {240, 244}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {218, 220}, {}, {}, {}, {}, {222, 224}, {232, 234}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {237, 240}, {}, {}, {}, {153, 156}, {}, {}, {}, {}, {220, 223}, {}, {161, 163}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {138, 139}, {}, {}, {}, {225, 228}, {}, {239, 242}, {}, {}, {}, {}, {}, {226, 230}, {225, 228}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {229, 231}, {237, 242}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {226, 229}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {202, 205}, {227, 230}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {232, 235}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {224, 227}, {245, 249}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {245, 249}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {221, 224}, {}, {}, {}, {}, {225, 228}, {235, 238}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {241, 245}, {}, {}, {}, {157, 160}, {}, {}, {}, {}, {224, 227}, {}, {164, 166}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {140, 141}, {}, {}, {}, {229, 232}, {}, {243, 247}, {}, {}, {}, {}, {}, {231, 235}, {229, 232}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {232, 235}, {243, 248}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {230, 234}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {206, 211}, {231, 233}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {236, 240}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {228, 232}, {250, 253}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {250, 253}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {12, 14}, {}, {}, {}, {33, 35}, {}, {}, {}, {}, {}, {42, 42}, {}, {}, {}, {}, {17, 19}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {37, 39}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {20, 23}, {}, {33, 36}, {}, {42, 45}, {}, {19, 22}, {}, {}, {}, {43, 47}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {27, 30}, {}, {}, {}, {43, 45}, {}, {41, 45}, {}, {}, {}, {}, {27, 29}, {}, {}, {}, {30, 31}, {}, {}, {}, {}, {34, 35}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {24, 26}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {31, 33}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {38, 41}, {}, {}, {}, {}, {}, {30, 31}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {16, 18}, {}, {}, {}, {22, 26}, {22, 27}, {}, {31, 33}, {}, {}, {}, {}, {}, {}, {18, 19}, {}, {}, {}, {17, 19}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {23, 26}, {}, {}, {}, {}, {30, 32}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {28, 31}, {}, {}, {}, {45, 48}, {}, {}, {}, {}, {}, {49, 50}, {}, {}, {}, {}, {30, 31}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {50, 53}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {35, 38}, {}, {46, 49}, {}, {58, 60}, {}, {36, 39}, {}, {}, {}, {61, 64}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {41, 43}, {}, {}, {}, {55, 57}, {}, {60, 63}, {}, {}, {}, {}, {35, 36}, {}, {}, {}, {37, 38}, {}, {}, {}, {}, {49, 51}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {31, 33}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {47, 50}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {49, 52}, {}, {}, {}, {}, {}, {40, 41}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {31, 33}, {}, {}, {}, {40, 44}, {}, {}, {47, 50}, {}, {}, {}, {}, {}, {}, {26, 28}, {}, {}, {}, {30, 32}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {33, 36}, {}, {}, {}, {}, {43, 46}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {45, 47}, {}, {}, {}, {}, {94, 97}, {66, 69}, {66, 69}, {50, 53}, {}, {}, {}, {}, {}, {}, {49, 54}, {}, {}, {}, {}, {}, {102, 107}, {62, 66}, {111, 114}, {}, {}, {}, {}, {69, 71}, {67, 75}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {55, 61}, {}, {}, {88, 92}, {85, 89}, {}, {}, {}, {111, 114}, {93, 98}, {}, {89, 94}, {92, 96}, {}, {}, {}, {}, {98, 103}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 55}, {}, {86, 89}, {}, {}, {}, {}, {99, 102}, {}, {}, {}, {}, {}, {}, {}, {}, {74, 80}, {}, {}, {}, {}, {93, 97}, {83, 88}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {89, 94}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {51, 54}, {}, {}, {}, {}, {53, 58}, {}, {}, {}, {}, {}, {}, {}, {}, {44, 45}, {}, {}, {}, {48, 52}, {}, {}, {}, {33, 36}, {}, {}, {}, {}, {}, {99, 101}, {}, {98, 101}, {}, {}, {}, {54, 57}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {32, 34}, {}, {28, 30}, {30, 32}, {}, {}, {}, {}, {}, {40, 41}, {}, {}, {}, {}, {}, {}, {}, {29, 30}, {}, {}, {43, 47}, {}, {}, {}, {}, {}, {29, 32}, {}, {25, 27}, {}, {}, {}, {}, {}, {}, {}, {}, {28, 31}, {28, 30}, {32, 33}, {}, {}, {}, {}, {}, {24, 27}, {}, {}, {}, {}, {}, {17, 18}, {35, 36}, {31, 34}, {}, {}, {16, 18}, {25, 28}, {}, {38, 41}, {}, {}, {}, {45, 48}, {}, {33, 37}, {}, {}, {}, {}, {}, {}, {24, 27}, {}, {}, {}, {21, 23}, {}, {}, {}, {40, 42}, {}, {33, 37}, {}, {}, {}, {33, 34}, {}, {}, {}, {}, {28, 29}, {}, {}, {42, 43}, {30, 33}, {}, {}, {}, {26, 28}, {32, 37}, {45, 48}, {}, {}, {}, {}, {27, 29}, {20, 21}, {}, {21, 22}, {33, 36}, {}, {}, {25, 27}, {26, 28}, {}, {}, {20, 21}, {}, {25, 26}, {}, {}, {}, {}, {}, {51, 53}, {}, {}, {45, 47}, {}, {38, 40}, {}, {}, {}, {}, {23, 26}, {}, {}, {44, 46}, {}, {30, 32}, {}, {41, 43}, {}, {}, {32, 34}, {}, {}, {51, 53}, {}, {}, {}, {}, {39, 41}, {}, {31, 33}, {}, {}, {37, 39}, {}, {}, {}, {}, {}, {25, 26}, {}, {}, {}, {}, {}, {}, {16, 17}, {}, {}, {47, 49}, {}, {}, {}, {}, {}, {38, 40}, {}, {}, {}, {}, {26, 30}, {30, 32}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {24, 26}, {}, {}, {}, {}, {37, 40}, {33, 37}, {}}},
    {{{}, {}, {}, {}, {}, {44, 46}, {}, {42, 45}, {42, 44}, {}, {}, {}, {}, {}, {47, 48}, {}, {}, {}, {}, {}, {}, {}, {39, 40}, {}, {}, {54, 57}, {}, {}, {}, {}, {}, {43, 46}, {}, {38, 41}, {}, {}, {}, {}, {}, {}, {}, {}, {49, 52}, {38, 40}, {42, 43}, {}, {}, {}, {}, {}, {36, 39}, {}, {}, {}, {}, {}, {25, 26}, {44, 45}, {43, 46}, {}, {}, {30, 31}, {39, 41}, {}, {54, 57}, {}, {}, {}, {56, 59}, {}, {55, 56}, {}, {}, {}, {}, {}, {}, {39, 42}, {}, {}, {}, {35, 37}, {}, {}, {}, {51, 54}, {}, {52, 55}, {}, {}, {}, {45, 48}, {}, {}, {}, {}, {35, 36}, {}, {}, {52, 53}, {38, 41}, {}, {}, {}, {39, 41}, {53, 55}, {56, 59}, {}, {}, {}, {}, {36, 37}, {27, 28}, {}, {28, 29}, {45, 48}, {}, {}, {35, 37}, {36, 37}, {}, {}, {26, 26}, {}, {38, 41}, {}, {}, {}, {}, {}, {65, 68}, {}, {}, {62, 64}, {}, {45, 49}, {}, {}, {}, {}, {36, 39}, {}, {}, {55, 57}, {}, {45, 47}, {}, {50, 52}, {}, {}, {47, 49}, {}, {}, {69, 71}, {}, {}, {}, {}, {47, 49}, {}, {45, 48}, {}, {}, {49, 51}, {}, {}, {}, {}, {}, {38, 41}, {}, {}, {}, {}, {}, {}, {23, 25}, {}, {}, {61, 63}, {}, {}, {}, {}, {}, {47, 49}, {}, {}, {}, {}, {51, 54}, {41, 43}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {37, 39}, {}, {}, {}, {}, {48, 51}, {51, 56}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {53, 57}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {77, 84}, {}, {36, 44}, {}, {}, {}, {}, {}, {49, 54}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {39, 42}, {}, {}, {}, {}, {}, {}, {88, 95}, {38, 46}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {56, 59}, {}, {}, {}, {}, {}, {}, {}, {}, {69, 74}, {}, {}, {}, {49, 52}, {65, 71}, {87, 94}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {35, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 86}, {}, {}, {}, {}, {47, 49}, {}, {}, {90, 95}, {}, {55, 58}, {}, {82, 89}, {}, {}, {56, 58}, {}, {}, {}, {}, {}, {}, {}, {73, 76}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {46, 50}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {66, 73}, {22, 28}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {87, 97}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {68, 73}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {87, 91}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {78, 82}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {103, 122}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {73, 89}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {88, 103}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {85, 97}, {109, 138}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {80, 106}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {80, 94}, {}, {}, {}, {}, {92, 108}, {}, {}, {}, {}, {97, 114}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {83, 98}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {184, 193}, {}, {}, {137, 139}, {137, 139}, {}, {}, {}, {}, {}, {135, 138}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {140, 152}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {138, 145}, {}, {}, {}, {}, {}, {}, {}, {130, 140}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {155, 163}, {}, {}, {}, {}, {}, {}, {153, 156}, {119, 121}, {}, {}, {}, {131, 133}, {149, 161}, {214, 225}, {}, {}, {}, {}, {201, 205}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {172, 186}, {}, {}, {}, {}, {}, {}, {}, {}, {151, 154}, {97, 99}, {122, 126}, {142, 148}, {160, 165}, {}, {}, {}, {}, {}, {145, 153}, {}, {}, {}, {139, 141}, {174, 184}, {}, {}, {137, 140}, {153, 154}, {173, 186}, {}, {}, {158, 164}, {131, 135}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {153, 164}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {161, 166}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {128, 138}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {194, 201}, {}, {}, {140, 141}, {140, 141}, {}, {}, {}, {}, {}, {139, 140}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {153, 160}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {146, 152}, {}, {}, {}, {}, {}, {}, {}, {141, 151}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {164, 166}, {}, {}, {}, {}, {}, {}, {157, 160}, {122, 123}, {}, {}, {}, {134, 137}, {162, 174}, {226, 231}, {}, {}, {}, {}, {206, 208}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {187, 199}, {}, {}, {}, {}, {}, {}, {}, {}, {155, 156}, {100, 102}, {127, 132}, {149, 153}, {166, 167}, {}, {}, {}, {}, {}, {154, 162}, {}, {}, {}, {142, 145}, {185, 193}, {}, {}, {141, 145}, {155, 169}, {187, 193}, {}, {}, {165, 169}, {136, 138}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {165, 173}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {167, 170}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {202, 213}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {141, 142}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {153, 162}, {}, {}, {}, {}, {}, {}, {}, {152, 160}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {167, 175}, {}, {}, {}, {}, {}, {}, {161, 165}, {124, 125}, {}, {}, {}, {138, 140}, {175, 184}, {232, 240}, {}, {}, {}, {}, {209, 211}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {200, 207}, {}, {}, {}, {}, {}, {}, {}, {}, {157, 158}, {103, 104}, {133, 134}, {154, 157}, {168, 169}, {}, {}, {}, {}, {}, {163, 171}, {}, {}, {}, {146, 147}, {194, 198}, {}, {}, {146, 149}, {170, 171}, {194, 203}, {}, {}, {170, 174}, {139, 143}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {174, 183}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {171, 174}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {49, 49}, {49, 49}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {55, 67}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {58, 64}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {39, 51}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {50, 52}, {50, 52}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {68, 80}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {65, 69}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {52, 63}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {53, 54}, {53, 54}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {81, 90}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {70, 74}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {64, 74}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {55, 55}, {55, 55}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {91, 105}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {75, 80}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {75, 86}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {60, 65}, {60, 65}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {106, 110}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {81, 92}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {87, 97}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {127, 127}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {85, 89}, {93, 96}, {98, 99}, {}, {101, 104}, {124, 126}, {}, {}, {}, {}, {}, {}, {138, 139}, {}, {}, {}, {90, 93}, {}, {74, 76}, {}, {105, 108}, {}, {116, 117}, {}, {}, {}, {}, {}, {}, {}, {}, {159, 159}, {}, {}, {}, {}, {}, {}, {}, {}, {136, 141}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {121, 129}, {}, {}, {}, {}, {}, {149, 158}, {}, {}, {}, {}, {98, 99}, {}, {111, 111}, {76, 77}, {}, {}, {}, {}, {}, {107, 107}, {}, {}, {}, {}, {}, {}, {63, 64}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {72, 78}, {145, 145}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {67, 74}, {}, {}, {}, {}, {}, {143, 145}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {89, 91}, {}, {37, 40}, {}, {}, {130, 130}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {6, 11}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {5, 11}, {4, 7}, {}, {}, {}, {5, 9}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {5, 10}, {3, 6}, {4, 7}, {5, 9}, {6, 10}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 5}, {6, 12}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {8, 13}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {12, 17}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {12, 18}, {8, 12}, {}, {}, {}, {10, 16}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {11, 14}, {7, 11}, {8, 12}, {10, 14}, {11, 18}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {6, 10}, {13, 20}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {14, 19}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {18, 25}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {19, 27}, {13, 20}, {}, {}, {}, {17, 23}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {15, 19}, {12, 15}, {13, 20}, {15, 23}, {19, 27}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {11, 22}, {21, 31}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {20, 27}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 9}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 9}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 5}, {}, {0, 6}, {}, {}, {}, {}, {}, {0, 3}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 6}, {0, 6}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 5}, {}, {}, {}, {}, {}, {0, 4}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 7}, {}, {}, {}, {}, {}, {}, {}, {0, 5}, {}, {}, {}, {0, 4}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 3}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 6}, {0, 4}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {158, 163}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {177, 182}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {138, 145}, {}, {}, {}, {}, {}, {174, 181}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {162, 167}, {}, {}, {}, {}, {}, {}, {}, {160, 164}, {}, {}, {}, {151, 156}, {}, {}, {}, {}, {}, {118, 120}, {}, {}, {}, {}, {132, 135}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {144, 151}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {12, 19}, {11, 16}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {73, 76}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {65, 67}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {46, 48}, {}, {}, {}, {}, {}, {51, 54}, {54, 56}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {77, 78}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {68, 70}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {49, 50}, {}, {}, {}, {}, {}, {55, 57}, {57, 60}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 81}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {71, 73}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {51, 53}, {}, {}, {}, {}, {}, {58, 60}, {61, 64}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {82, 84}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {74, 77}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {54, 56}, {}, {}, {}, {}, {}, {61, 62}, {65, 65}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 4}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {5, 9}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {10, 11}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {12, 13}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {79, 81}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
}};

// 0x8BA4D0
static constexpr notsa::mdarray<tSpecificSpeechContextInfo, CTX_EMG_END, VOICE_EMG_END> gEmgSpeechLookup{{
    {{{0, 2}, {0, 2}, {0, 4}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{3, 4}, {3, 4}, {5, 6}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{5, 6}, {5, 6}, {7, 8}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{7, 9}, {7, 10}, {9, 12}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {34, 36}, {}, {37, 40}, {27, 31}, {23, 28}, {35, 40}, {41, 49}, {26, 31}, {36, 44}, {39, 44}, {}, {}, {}, {24, 27}, {22, 26}, {22, 23}, {19, 22}, {24, 28}, {22, 25}, {}, {29, 34}, {38, 42}, {37, 39}, {39, 42}, {26, 28}, {34, 38}, {31, 39}, {30, 33}, {36, 43}, {}, {}, {}, {}}},
    {{{10, 11}, {11, 12}, {13, 14}, {}, {}, {}, {}, {}, {0, 4}, {0, 7}, {0, 6}, {0, 5}, {0, 5}, {37, 42}, {24, 28}, {41, 47}, {32, 36}, {29, 32}, {41, 48}, {50, 53}, {32, 35}, {45, 51}, {45, 51}, {31, 35}, {26, 29}, {23, 28}, {28, 31}, {27, 31}, {24, 27}, {23, 25}, {29, 33}, {26, 29}, {}, {35, 36}, {43, 46}, {40, 44}, {43, 48}, {29, 32}, {39, 46}, {40, 43}, {34, 40}, {44, 51}, {5, 10}, {0, 6}, {0, 3}, {0, 2}}},
    {{{12, 15}, {13, 16}, {15, 18}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{16, 17}, {17, 18}, {19, 20}, {15, 20}, {14, 21}, {18, 23}, {11, 14}, {9, 12}, {17, 24}, {19, 26}, {24, 31}, {16, 24}, {17, 21}, {54, 56}, {36, 39}, {58, 66}, {45, 49}, {46, 50}, {62, 69}, {65, 69}, {45, 48}, {60, 67}, {64, 68}, {47, 52}, {45, 49}, {38, 42}, {43, 47}, {42, 45}, {39, 43}, {30, 33}, {43, 46}, {40, 44}, {}, {46, 57}, {60, 65}, {58, 65}, {63, 68}, {40, 46}, {57, 64}, {50, 56}, {51, 53}, {66, 69}, {11, 15}, {7, 12}, {4, 8}, {3, 6}}},
    {{{18, 19}, {19, 20}, {21, 22}, {}, {}, {}, {}, {}, {25, 29}, {27, 30}, {32, 35}, {25, 27}, {22, 25}, {57, 62}, {40, 44}, {67, 73}, {50, 53}, {51, 55}, {70, 76}, {70, 74}, {49, 53}, {68, 74}, {69, 76}, {53, 57}, {50, 54}, {43, 46}, {48, 53}, {46, 51}, {44, 49}, {34, 38}, {47, 52}, {45, 49}, {0, 3}, {58, 65}, {66, 71}, {66, 71}, {69, 74}, {47, 53}, {65, 70}, {57, 62}, {54, 60}, {70, 78}, {16, 21}, {13, 17}, {9, 12}, {7, 9}}},
    {{{20, 23}, {21, 24}, {23, 26}, {21, 34}, {22, 31}, {24, 37}, {15, 23}, {13, 19}, {}, {}, {}, {}, {}, {63, 72}, {45, 53}, {74, 86}, {54, 60}, {56, 63}, {77, 89}, {75, 85}, {54, 64}, {75, 86}, {77, 87}, {58, 65}, {55, 66}, {47, 52}, {54, 61}, {52, 58}, {50, 56}, {39, 43}, {53, 58}, {50, 57}, {4, 9}, {66, 71}, {72, 81}, {72, 81}, {75, 82}, {54, 66}, {71, 79}, {63, 68}, {61, 70}, {79, 91}, {}, {}, {}, {}}},
    {{{24, 25}, {25, 26}, {27, 28}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{26, 27}, {27, 28}, {29, 30}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {82, 86}, {59, 63}, {95, 102}, {72, 75}, {74, 79}, {101, 105}, {95, 99}, {71, 74}, {97, 100}, {100, 106}, {}, {}, {}, {70, 74}, {67, 70}, {65, 68}, {50, 54}, {67, 70}, {64, 68}, {}, {82, 87}, {92, 96}, {91, 95}, {91, 95}, {}, {87, 90}, {}, {79, 87}, {103, 108}, {22, 28}, {}, {}, {}}},
    {{{28, 29}, {29, 30}, {31, 32}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{30, 31}, {31, 31}, {33, 34}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{32, 36}, {32, 37}, {35, 40}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {0, 9}, {0, 7}, {0, 11}, {0, 6}, {0, 8}, {5, 10}, {8, 14}, {7, 14}, {6, 11}, {6, 11}, {}, {}, {48, 57}, {37, 41}, {33, 40}, {49, 55}, {54, 61}, {}, {52, 55}, {52, 60}, {36, 42}, {30, 39}, {29, 33}, {}, {}, {}, {}, {}, {}, {}, {37, 42}, {47, 55}, {45, 52}, {49, 57}, {}, {47, 52}, {}, {41, 46}, {52, 61}, {}, {}, {}, {}}},
    {{{}, {}, {}, {10, 14}, {8, 13}, {12, 17}, {7, 10}, {}, {11, 16}, {15, 18}, {15, 23}, {12, 15}, {12, 16}, {43, 53}, {29, 35}, {}, {42, 44}, {41, 45}, {56, 61}, {62, 64}, {36, 44}, {56, 59}, {61, 63}, {43, 46}, {40, 44}, {34, 37}, {39, 42}, {38, 41}, {35, 38}, {29, 29}, {39, 42}, {36, 39}, {}, {43, 45}, {56, 59}, {53, 57}, {58, 62}, {33, 39}, {53, 56}, {44, 49}, {47, 50}, {62, 65}, {}, {}, {}, {}}},
    {{{}, {}, {}, {35, 39}, {32, 35}, {38, 43}, {24, 27}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {40, 49}, {36, 49}, {44, 55}, {28, 41}, {20, 27}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {50, 61}, {50, 61}, {56, 67}, {42, 53}, {28, 36}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {30, 32}, {31, 40}, {42, 46}, {28, 34}, {31, 34}, {100, 103}, {71, 73}, {109, 114}, {87, 91}, {88, 91}, {122, 126}, {117, 120}, {86, 88}, {107, 114}, {119, 124}, {81, 85}, {90, 92}, {72, 74}, {83, 86}, {79, 82}, {76, 79}, {55, 57}, {78, 81}, {74, 77}, {19, 21}, {98, 102}, {102, 107}, {101, 106}, {101, 106}, {}, {99, 103}, {}, {110, 115}, {123, 130}, {41, 46}, {34, 38}, {22, 24}, {16, 18}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {33, 36}, {41, 50}, {47, 52}, {35, 41}, {35, 41}, {104, 109}, {74, 77}, {115, 125}, {92, 94}, {92, 94}, {127, 133}, {121, 125}, {89, 93}, {115, 120}, {125, 133}, {86, 92}, {93, 96}, {75, 80}, {87, 92}, {83, 88}, {80, 85}, {58, 61}, {82, 85}, {78, 82}, {22, 26}, {103, 107}, {108, 112}, {107, 110}, {107, 112}, {}, {104, 110}, {}, {116, 120}, {131, 139}, {47, 50}, {39, 44}, {25, 29}, {19, 21}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {36, 41}, {}, {26, 30}, {87, 88}, {64, 65}, {103, 108}, {76, 80}, {}, {106, 107}, {100, 101}, {75, 79}, {}, {107, 108}, {}, {77, 80}, {59, 64}, {}, {}, {}, {}, {}, {}, {13, 18}, {88, 89}, {}, {}, {}, {70, 71}, {}, {72, 73}, {88, 89}, {109, 110}, {}, {28, 33}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 8}, {0, 5}, {0, 5}, {0, 8}, {0, 4}, {0, 8}, {0, 9}, {0, 8}, {0, 8}, {0, 9}, {0, 5}, {0, 6}, {0, 7}, {0, 4}, {0, 3}, {0, 4}, {0, 2}, {0, 4}, {0, 3}, {}, {0, 10}, {0, 10}, {0, 10}, {0, 9}, {0, 2}, {0, 9}, {0, 7}, {0, 8}, {0, 8}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {9, 13}, {6, 9}, {6, 12}, {9, 11}, {5, 7}, {9, 12}, {10, 14}, {9, 10}, {9, 12}, {10, 14}, {6, 9}, {7, 12}, {8, 9}, {5, 8}, {}, {}, {3, 5}, {5, 9}, {4, 7}, {}, {}, {11, 13}, {11, 15}, {10, 14}, {3, 5}, {10, 13}, {8, 11}, {9, 12}, {9, 13}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {14, 17}, {10, 13}, {13, 17}, {12, 14}, {8, 10}, {13, 16}, {15, 21}, {11, 13}, {13, 17}, {15, 20}, {10, 15}, {}, {10, 12}, {9, 12}, {8, 11}, {9, 12}, {6, 9}, {10, 13}, {8, 11}, {}, {11, 14}, {14, 21}, {16, 21}, {15, 21}, {6, 11}, {14, 19}, {12, 16}, {13, 16}, {14, 18}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {18, 21}, {14, 17}, {18, 24}, {15, 18}, {11, 14}, {17, 23}, {22, 27}, {14, 16}, {18, 23}, {21, 27}, {16, 19}, {13, 17}, {13, 17}, {13, 17}, {12, 15}, {13, 16}, {10, 13}, {14, 17}, {12, 15}, {}, {15, 19}, {22, 25}, {22, 25}, {22, 25}, {12, 16}, {20, 23}, {}, {17, 20}, {19, 26}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {22, 33}, {18, 23}, {25, 36}, {19, 26}, {15, 22}, {24, 34}, {28, 40}, {17, 25}, {24, 35}, {28, 38}, {20, 30}, {18, 25}, {18, 22}, {18, 23}, {16, 21}, {17, 21}, {14, 18}, {18, 23}, {16, 21}, {}, {20, 28}, {26, 37}, {26, 36}, {26, 38}, {17, 25}, {24, 33}, {17, 30}, {21, 29}, {27, 35}, {0, 4}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {73, 75}, {54, 55}, {87, 89}, {61, 64}, {64, 66}, {90, 94}, {86, 89}, {65, 65}, {87, 90}, {88, 92}, {66, 69}, {67, 72}, {53, 55}, {62, 64}, {59, 60}, {57, 58}, {44, 46}, {59, 60}, {58, 60}, {}, {72, 75}, {82, 84}, {82, 84}, {83, 85}, {}, {80, 81}, {}, {71, 73}, {92, 95}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {76, 81}, {56, 58}, {90, 94}, {65, 71}, {67, 73}, {95, 100}, {90, 94}, {66, 70}, {91, 96}, {93, 99}, {70, 74}, {73, 76}, {56, 58}, {65, 69}, {61, 66}, {59, 64}, {47, 49}, {61, 66}, {61, 63}, {10, 12}, {76, 81}, {85, 91}, {85, 90}, {86, 90}, {67, 69}, {82, 86}, {69, 71}, {74, 78}, {96, 102}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {89, 99}, {66, 70}, {}, {81, 86}, {80, 87}, {108, 118}, {102, 110}, {80, 85}, {101, 106}, {109, 118}, {75, 80}, {81, 89}, {65, 71}, {75, 82}, {71, 78}, {69, 75}, {}, {71, 77}, {69, 73}, {}, {90, 97}, {97, 101}, {96, 100}, {96, 100}, {}, {91, 98}, {}, {90, 102}, {111, 122}, {36, 40}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {119, 121}, {111, 116}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {103, 109}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {32, 38}, {32, 37}, {28, 34}, {26, 28}, {34, 38}, {30, 35}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {4, 7}, {5, 8}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {29, 35}, {18, 27}, {13, 21}, {10, 15}}},
}};

// 0x8BBEF8
static constexpr notsa::mdarray<tSpecificSpeechContextInfo, CTX_PLY_END, VOICE_PLY_END> gPlySpeechLookup{{
    {{{0, 2}, {0, 1}, {0, 1}, {0, 1}, {7, 9}, {6, 9}, {8, 12}, {8, 11}, {8, 11}, {10, 13}, {8, 11}, {9, 14}, {}, {}, {}, {}, {7, 10}, {8, 10}, {8, 15}, {8, 15}}},
    {{{3, 4}, {2, 3}, {2, 4}, {2, 4}, {14, 16}, {14, 15}, {18, 20}, {16, 18}, {16, 18}, {17, 18}, {16, 20}, {19, 20}, {}, {}, {0, 2}, {0, 1}, {15, 17}, {15, 16}, {21, 22}, {21, 22}}},
    {{{5, 7}, {4, 6}, {}, {}, {17, 24}, {16, 22}, {21, 29}, {19, 27}, {19, 27}, {19, 29}, {21, 26}, {21, 28}, {}, {}, {3, 9}, {2, 6}, {}, {}, {23, 26}, {23, 25}}},
    {{{8, 14}, {7, 14}, {5, 15}, {5, 14}, {53, 62}, {47, 55}, {56, 65}, {51, 60}, {48, 60}, {50, 60}, {49, 61}, {47, 62}, {}, {}, {10, 24}, {7, 21}, {}, {}, {}, {}}},
    {{{15, 23}, {15, 23}, {16, 25}, {15, 24}, {73, 80}, {65, 71}, {74, 83}, {69, 76}, {67, 78}, {67, 77}, {69, 80}, {71, 84}, {}, {}, {25, 31}, {22, 28}, {23, 26}, {21, 25}, {36, 42}, {33, 37}}},
    {{{24, 29}, {24, 29}, {26, 32}, {25, 30}, {81, 86}, {72, 77}, {84, 91}, {77, 83}, {79, 86}, {78, 85}, {81, 85}, {85, 92}, {}, {}, {32, 36}, {29, 32}, {27, 30}, {26, 29}, {43, 45}, {38, 41}}},
    {{{30, 37}, {30, 36}, {33, 38}, {31, 35}, {89, 97}, {79, 86}, {94, 103}, {86, 95}, {89, 93}, {87, 91}, {89, 93}, {95, 99}, {}, {}, {}, {}, {32, 38}, {31, 36}, {48, 56}, {43, 50}}},
    {{{38, 45}, {37, 41}, {39, 47}, {36, 43}, {98, 104}, {87, 94}, {104, 111}, {96, 102}, {99, 109}, {96, 107}, {103, 116}, {115, 132}, {}, {}, {37, 42}, {33, 38}, {39, 45}, {37, 43}, {57, 62}, {51, 56}}},
    {{{46, 50}, {42, 46}, {63, 70}, {59, 65}, {129, 136}, {118, 124}, {141, 147}, {128, 133}, {142, 153}, {138, 148}, {166, 173}, {185, 193}, {}, {}, {43, 49}, {39, 44}, {60, 64}, {57, 61}, {80, 83}, {72, 75}}},
    {{{51, 55}, {47, 51}, {71, 79}, {66, 73}, {137, 141}, {125, 129}, {148, 153}, {134, 138}, {154, 163}, {149, 157}, {174, 185}, {}, {}, {}, {50, 56}, {45, 50}, {65, 72}, {62, 68}, {84, 93}, {76, 84}}},
    {{{56, 61}, {52, 57}, {80, 85}, {74, 78}, {142, 146}, {130, 134}, {154, 160}, {139, 144}, {164, 172}, {158, 167}, {186, 195}, {194, 202}, {}, {}, {57, 61}, {51, 54}, {73, 77}, {69, 73}, {94, 97}, {85, 88}}},
    {{{62, 66}, {58, 62}, {}, {}, {147, 152}, {135, 139}, {}, {}, {173, 177}, {168, 171}, {}, {}, {}, {}, {}, {}, {78, 81}, {74, 77}, {}, {}}},
    {{{67, 71}, {63, 67}, {92, 96}, {84, 88}, {153, 158}, {140, 145}, {165, 171}, {149, 154}, {178, 186}, {172, 179}, {206, 214}, {212, 221}, {}, {}, {67, 71}, {59, 63}, {82, 87}, {78, 84}, {103, 108}, {93, 98}}},
    {{{72, 75}, {68, 71}, {}, {}, {159, 164}, {146, 151}, {172, 175}, {155, 158}, {187, 192}, {180, 184}, {215, 219}, {222, 226}, {}, {}, {}, {}, {88, 93}, {85, 87}, {109, 114}, {99, 104}}},
    {{{76, 81}, {72, 77}, {}, {}, {}, {}, {}, {}, {193, 197}, {185, 189}, {}, {}, {}, {}, {}, {}, {94, 100}, {88, 92}, {}, {}}},
    {{{82, 86}, {78, 81}, {}, {}, {}, {}, {}, {}, {198, 202}, {190, 192}, {}, {}, {}, {}, {}, {}, {101, 106}, {93, 97}, {}, {}}},
    {{{87, 92}, {82, 87}, {}, {}, {}, {}, {}, {}, {253, 257}, {237, 241}, {}, {}, {0, 3}, {0, 4}, {}, {}, {151, 156}, {140, 146}, {}, {}}},
    {{{93, 97}, {88, 92}, {}, {}, {}, {}, {}, {}, {258, 261}, {242, 245}, {}, {}, {4, 7}, {5, 7}, {}, {}, {157, 161}, {147, 152}, {}, {}}},
    {{{98, 102}, {93, 96}, {97, 101}, {89, 92}, {227, 232}, {209, 214}, {229, 231}, {204, 206}, {297, 302}, {274, 279}, {288, 295}, {286, 291}, {}, {}, {72, 76}, {64, 67}, {165, 169}, {154, 158}, {166, 170}, {159, 162}}},
    {{{103, 109}, {97, 106}, {102, 117}, {93, 108}, {233, 240}, {215, 221}, {232, 239}, {207, 214}, {303, 316}, {280, 288}, {296, 308}, {292, 303}, {}, {}, {77, 84}, {68, 75}, {170, 175}, {159, 162}, {171, 176}, {163, 168}}},
    {{{110, 116}, {107, 113}, {118, 130}, {109, 120}, {241, 248}, {222, 226}, {240, 244}, {215, 219}, {317, 324}, {289, 295}, {309, 322}, {304, 317}, {}, {}, {}, {}, {176, 181}, {163, 170}, {177, 181}, {169, 173}}},
    {{{117, 120}, {114, 116}, {131, 135}, {121, 124}, {249, 254}, {227, 232}, {245, 250}, {220, 223}, {325, 328}, {296, 299}, {323, 326}, {318, 321}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{121, 123}, {117, 119}, {136, 139}, {125, 127}, {255, 258}, {233, 236}, {251, 254}, {224, 226}, {329, 332}, {300, 303}, {327, 329}, {322, 325}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{124, 127}, {120, 123}, {146, 150}, {133, 137}, {276, 281}, {252, 256}, {270, 274}, {240, 243}, {353, 358}, {324, 328}, {350, 352}, {345, 348}, {}, {}, {}, {}, {193, 198}, {180, 184}, {196, 200}, {187, 191}}},
    {{{}, {}, {48, 57}, {44, 53}, {}, {}, {}, {}, {}, {}, {117, 121}, {133, 137}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {58, 62}, {54, 58}, {}, {}, {}, {}, {}, {}, {122, 127}, {138, 143}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {86, 91}, {79, 83}, {}, {}, {161, 164}, {145, 148}, {}, {}, {196, 205}, {203, 211}, {}, {}, {62, 66}, {55, 58}, {}, {}, {98, 102}, {89, 92}}},
    {{{}, {}, {140, 145}, {128, 132}, {270, 275}, {246, 251}, {264, 269}, {235, 239}, {347, 352}, {318, 323}, {346, 349}, {341, 344}, {}, {}, {}, {}, {188, 192}, {176, 179}, {191, 195}, {182, 186}}},
    {{{}, {}, {151, 156}, {138, 144}, {}, {}, {}, {}, {}, {}, {393, 399}, {394, 399}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {0, 2}, {0, 1}, {0, 1}, {0, 2}, {0, 2}, {0, 4}, {0, 2}, {0, 1}, {}, {}, {}, {}, {0, 1}, {0, 2}, {0, 2}, {0, 2}}},
    {{{}, {}, {}, {}, {3, 4}, {2, 3}, {2, 4}, {3, 5}, {3, 5}, {5, 7}, {3, 4}, {2, 5}, {}, {}, {}, {}, {2, 4}, {3, 5}, {3, 4}, {3, 5}}},
    {{{}, {}, {}, {}, {5, 6}, {4, 5}, {5, 7}, {6, 7}, {6, 7}, {8, 9}, {5, 7}, {6, 8}, {}, {}, {}, {}, {5, 6}, {6, 7}, {5, 7}, {6, 7}}},
    {{{}, {}, {}, {}, {10, 11}, {10, 11}, {13, 14}, {12, 13}, {12, 13}, {14, 15}, {12, 13}, {15, 16}, {}, {}, {}, {}, {11, 12}, {11, 12}, {16, 17}, {16, 17}}},
    {{{}, {}, {}, {}, {12, 13}, {12, 13}, {15, 17}, {14, 15}, {14, 15}, {16, 16}, {14, 15}, {17, 18}, {}, {}, {}, {}, {13, 14}, {13, 14}, {18, 20}, {18, 20}}},
    {{{}, {}, {}, {}, {25, 28}, {23, 24}, {30, 32}, {28, 29}, {28, 30}, {30, 32}, {27, 30}, {29, 31}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {29, 33}, {25, 28}, {33, 37}, {30, 34}, {31, 33}, {33, 36}, {31, 34}, {32, 34}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {34, 35}, {29, 30}, {38, 40}, {35, 36}, {34, 36}, {37, 39}, {35, 36}, {35, 36}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {36, 40}, {31, 35}, {41, 45}, {37, 40}, {37, 39}, {40, 42}, {37, 39}, {37, 39}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {41, 46}, {36, 41}, {46, 50}, {41, 46}, {40, 43}, {43, 45}, {43, 45}, {42, 43}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {47, 52}, {42, 46}, {51, 55}, {47, 50}, {44, 47}, {46, 49}, {46, 48}, {44, 46}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {63, 65}, {56, 58}, {66, 68}, {61, 63}, {61, 63}, {61, 63}, {62, 63}, {63, 67}, {}, {}, {}, {}, {18, 20}, {17, 18}, {27, 29}, {26, 26}}},
    {{{}, {}, {}, {}, {66, 69}, {59, 61}, {69, 70}, {64, 65}, {64, 66}, {64, 66}, {64, 66}, {68, 68}, {}, {}, {}, {}, {21, 22}, {19, 20}, {30, 32}, {27, 29}}},
    {{{}, {}, {}, {}, {70, 72}, {62, 64}, {71, 73}, {66, 68}, {}, {}, {67, 68}, {69, 70}, {}, {}, {}, {}, {}, {}, {33, 35}, {30, 32}}},
    {{{}, {}, {}, {}, {87, 88}, {78, 78}, {92, 93}, {84, 85}, {87, 88}, {86, 86}, {86, 88}, {93, 94}, {}, {}, {}, {}, {31, 31}, {30, 30}, {46, 47}, {42, 42}}},
    {{{}, {}, {}, {}, {105, 108}, {95, 98}, {112, 116}, {103, 106}, {110, 114}, {108, 112}, {128, 130}, {144, 148}, {}, {}, {}, {}, {46, 48}, {44, 45}, {63, 65}, {57, 58}}},
    {{{}, {}, {}, {}, {109, 112}, {99, 102}, {117, 121}, {107, 111}, {115, 119}, {113, 117}, {131, 133}, {149, 153}, {}, {}, {}, {}, {49, 51}, {46, 48}, {66, 68}, {59, 60}}},
    {{{}, {}, {}, {}, {113, 118}, {103, 108}, {122, 129}, {112, 118}, {120, 129}, {118, 127}, {134, 140}, {154, 159}, {}, {}, {}, {}, {52, 53}, {49, 51}, {69, 70}, {61, 62}}},
    {{{}, {}, {}, {}, {119, 126}, {109, 115}, {130, 137}, {119, 125}, {130, 139}, {128, 136}, {141, 146}, {160, 165}, {}, {}, {}, {}, {54, 57}, {52, 54}, {71, 74}, {63, 66}}},
    {{{}, {}, {}, {}, {127, 128}, {116, 117}, {138, 140}, {126, 127}, {140, 141}, {137, 137}, {153, 155}, {172, 174}, {}, {}, {}, {}, {58, 59}, {55, 56}, {77, 79}, {70, 71}}},
    {{{}, {}, {}, {}, {165, 168}, {152, 154}, {176, 178}, {159, 160}, {203, 206}, {193, 195}, {220, 222}, {227, 229}, {}, {}, {}, {}, {107, 109}, {98, 98}, {115, 115}, {105, 107}}},
    {{{}, {}, {}, {}, {169, 171}, {155, 157}, {179, 181}, {161, 163}, {207, 209}, {196, 198}, {223, 226}, {230, 232}, {}, {}, {}, {}, {110, 111}, {99, 101}, {116, 117}, {108, 110}}},
    {{{}, {}, {}, {}, {172, 178}, {158, 164}, {182, 186}, {164, 167}, {210, 217}, {199, 205}, {227, 233}, {233, 238}, {}, {}, {}, {}, {112, 117}, {102, 106}, {118, 122}, {111, 115}}},
    {{{}, {}, {}, {}, {179, 187}, {165, 172}, {187, 191}, {168, 172}, {218, 220}, {206, 208}, {234, 240}, {239, 246}, {}, {}, {}, {}, {118, 123}, {107, 113}, {123, 128}, {116, 119}}},
    {{{}, {}, {}, {}, {188, 190}, {173, 175}, {192, 194}, {173, 174}, {221, 224}, {209, 211}, {241, 243}, {247, 250}, {}, {}, {}, {}, {124, 124}, {114, 115}, {129, 130}, {120, 121}}},
    {{{}, {}, {}, {}, {191, 197}, {176, 182}, {195, 199}, {175, 179}, {225, 229}, {212, 216}, {244, 249}, {251, 252}, {}, {}, {}, {}, {125, 129}, {116, 120}, {131, 136}, {122, 127}}},
    {{{}, {}, {}, {}, {198, 204}, {183, 189}, {200, 203}, {180, 183}, {230, 234}, {217, 221}, {250, 254}, {253, 254}, {}, {}, {}, {}, {130, 134}, {121, 125}, {137, 143}, {128, 134}}},
    {{{}, {}, {}, {}, {205, 206}, {190, 192}, {204, 207}, {184, 186}, {235, 237}, {222, 223}, {255, 257}, {255, 257}, {}, {}, {}, {}, {135, 137}, {126, 127}, {144, 148}, {135, 139}}},
    {{{}, {}, {}, {}, {207, 210}, {193, 195}, {208, 210}, {187, 188}, {238, 241}, {224, 227}, {258, 261}, {258, 261}, {}, {}, {}, {}, {138, 140}, {128, 130}, {149, 151}, {140, 143}}},
    {{{}, {}, {}, {}, {211, 214}, {196, 198}, {211, 213}, {189, 191}, {242, 243}, {228, 228}, {262, 264}, {262, 263}, {}, {}, {}, {}, {141, 143}, {131, 132}, {152, 155}, {144, 146}}},
    {{{}, {}, {}, {}, {215, 218}, {199, 201}, {214, 217}, {192, 194}, {244, 247}, {229, 231}, {265, 267}, {264, 266}, {}, {}, {}, {}, {144, 146}, {133, 135}, {156, 157}, {147, 149}}},
    {{{}, {}, {}, {}, {219, 220}, {202, 203}, {218, 220}, {195, 196}, {248, 250}, {232, 234}, {271, 273}, {270, 271}, {}, {}, {}, {}, {147, 148}, {136, 137}, {158, 159}, {150, 151}}},
    {{{}, {}, {}, {}, {221, 222}, {204, 205}, {221, 222}, {197, 198}, {}, {}, {274, 276}, {272, 273}, {}, {}, {}, {}, {}, {}, {160, 160}, {152, 153}}},
    {{{}, {}, {}, {}, {223, 224}, {206, 207}, {223, 224}, {199, 200}, {251, 252}, {235, 236}, {277, 279}, {274, 276}, {}, {}, {}, {}, {149, 150}, {138, 139}, {161, 162}, {154, 156}}},
    {{{}, {}, {}, {}, {225, 226}, {208, 208}, {225, 228}, {201, 203}, {294, 296}, {272, 273}, {280, 282}, {277, 279}, {}, {}, {}, {}, {162, 164}, {153, 153}, {163, 165}, {157, 158}}},
    {{{}, {}, {}, {}, {259, 263}, {237, 240}, {255, 259}, {227, 231}, {333, 336}, {304, 307}, {337, 339}, {333, 336}, {}, {}, {}, {}, {182, 187}, {171, 175}, {182, 185}, {174, 177}}},
    {{{}, {}, {}, {}, {264, 269}, {241, 245}, {260, 263}, {232, 234}, {337, 346}, {308, 317}, {340, 345}, {337, 340}, {}, {}, {}, {}, {}, {}, {186, 190}, {178, 181}}},
    {{{}, {}, {}, {}, {282, 284}, {257, 258}, {275, 278}, {244, 246}, {359, 360}, {329, 330}, {387, 389}, {386, 388}, {}, {}, {}, {}, {199, 200}, {185, 186}, {201, 202}, {192, 195}}},
    {{{}, {}, {}, {}, {285, 288}, {259, 262}, {279, 281}, {247, 249}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {289, 292}, {263, 266}, {282, 284}, {250, 252}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {94, 98}, {92, 95}, {94, 102}, {100, 114}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {262, 265}, {246, 249}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {266, 267}, {250, 251}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {268, 270}, {252, 253}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {271, 272}, {254, 255}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {273, 275}, {256, 256}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {276, 278}, {257, 258}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {279, 280}, {259, 260}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {281, 282}, {261, 261}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {283, 284}, {262, 263}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {285, 287}, {264, 265}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {288, 290}, {266, 268}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {291, 293}, {269, 271}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {40, 42}, {40, 41}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {147, 149}, {166, 168}, {}, {}, {}, {}, {}, {}, {75, 75}, {67, 67}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {150, 152}, {169, 171}, {}, {}, {}, {}, {}, {}, {76, 76}, {68, 69}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {156, 160}, {175, 179}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {161, 165}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {268, 270}, {267, 269}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {283, 284}, {280, 282}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {285, 287}, {283, 285}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {330, 336}, {326, 332}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {353, 356}, {349, 352}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {357, 360}, {353, 355}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {361, 364}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {365, 368}, {359, 363}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {369, 369}, {364, 366}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {370, 370}, {367, 367}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {371, 371}, {368, 368}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {372, 373}, {369, 369}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {374, 375}, {370, 371}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {376, 377}, {372, 372}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {378, 378}, {373, 374}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {379, 379}, {375, 376}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {380, 380}, {377, 378}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {381, 381}, {379, 380}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {382, 382}, {381, 382}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {383, 383}, {383, 383}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {384, 386}, {384, 385}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {390, 392}, {389, 393}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {180, 184}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {356, 358}, {}, {}, {}, {}, {}, {}, {}, {}}},
}};

// 0x8BE620
static constexpr notsa::mdarray<tSpecificSpeechContextInfo, CTX_GNG_END, VOICE_GNG_END> gGngSpeechLookup{{
    {{{0, 6}, {0, 5}, {0, 4}, {0, 3}, {25, 29}, {}, {9, 12}, {20, 23}, {17, 20}, {17, 19}, {13, 15}, {}, {22, 25}, {18, 21}, {18, 21}, {15, 18}, {15, 18}, {}, {33, 36}, {39, 42}, {29, 32}, {32, 34}, {27, 30}, {}, {}, {}, {0, 2}, {7, 10}, {14, 17}, {10, 14}, {10, 11}, {12, 13}, {0, 3}, {2, 4}, {4, 6}, {3, 4}, {2, 5}, {}, {}, {0, 2}, {}, {26, 29}, {23, 26}, {25, 28}, {25, 28}, {24, 27}, {3, 5}, {2, 3}, {3, 3}, {0, 2}, {3, 4}, {}}},
    {{{7, 15}, {6, 20}, {5, 14}, {4, 16}, {30, 49}, {}, {}, {24, 37}, {21, 33}, {20, 31}, {16, 28}, {}, {}, {}, {}, {}, {}, {}, {37, 46}, {43, 55}, {33, 42}, {35, 46}, {31, 40}, {}, {}, {}, {}, {11, 30}, {18, 27}, {15, 27}, {12, 17}, {14, 19}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {30, 39}, {27, 41}, {29, 42}, {29, 36}, {28, 41}, {}, {}, {}, {}, {}, {}}},
    {{{16, 21}, {21, 24}, {15, 18}, {17, 20}, {50, 53}, {}, {}, {47, 50}, {40, 43}, {}, {37, 38}, {}, {32, 35}, {45, 48}, {49, 52}, {40, 43}, {39, 42}, {}, {47, 50}, {66, 68}, {52, 55}, {54, 56}, {48, 52}, {}, {}, {}, {}, {38, 42}, {36, 38}, {35, 37}, {24, 25}, {26, 28}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {49, 51}, {50, 51}, {51, 53}, {48, 51}, {47, 49}, {}, {}, {}, {}, {}, {}}},
    {{{22, 27}, {25, 31}, {19, 25}, {21, 27}, {54, 60}, {}, {21, 25}, {51, 58}, {44, 51}, {40, 45}, {39, 45}, {}, {36, 43}, {49, 54}, {53, 60}, {44, 48}, {43, 48}, {0, 1}, {51, 58}, {69, 75}, {56, 62}, {57, 61}, {53, 57}, {}, {0, 3}, {}, {3, 5}, {43, 49}, {39, 44}, {38, 42}, {26, 29}, {29, 35}, {4, 7}, {9, 13}, {12, 16}, {5, 9}, {6, 11}, {}, {}, {}, {0, 4}, {52, 57}, {52, 58}, {54, 60}, {52, 57}, {50, 56}, {6, 10}, {4, 7}, {4, 7}, {3, 6}, {5, 9}, {0, 1}}},
    {{{28, 32}, {32, 35}, {26, 29}, {28, 31}, {61, 63}, {}, {}, {59, 61}, {52, 54}, {46, 49}, {46, 49}, {}, {44, 47}, {55, 58}, {}, {49, 53}, {49, 53}, {}, {59, 62}, {76, 78}, {63, 66}, {62, 64}, {58, 60}, {}, {}, {}, {}, {50, 53}, {45, 48}, {43, 46}, {30, 32}, {36, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {58, 60}, {59, 61}, {61, 63}, {58, 60}, {57, 59}, {}, {}, {}, {}, {}, {}}},
    {{{33, 37}, {36, 40}, {30, 33}, {32, 36}, {64, 66}, {}, {}, {62, 64}, {55, 57}, {50, 53}, {50, 52}, {}, {48, 51}, {59, 62}, {61, 63}, {54, 58}, {54, 58}, {}, {63, 66}, {79, 81}, {67, 70}, {65, 67}, {61, 63}, {}, {}, {}, {}, {54, 56}, {49, 50}, {47, 49}, {33, 35}, {39, 41}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {61, 63}, {62, 64}, {64, 66}, {61, 63}, {60, 62}, {}, {}, {}, {}, {}, {}}},
    {{{38, 43}, {41, 46}, {34, 39}, {37, 42}, {67, 72}, {16, 21}, {26, 34}, {65, 72}, {58, 65}, {54, 62}, {53, 57}, {0, 3}, {52, 59}, {63, 70}, {64, 71}, {59, 66}, {59, 66}, {2, 7}, {67, 75}, {82, 88}, {71, 80}, {68, 75}, {64, 73}, {}, {4, 6}, {0, 3}, {6, 10}, {57, 64}, {51, 58}, {50, 55}, {36, 41}, {42, 48}, {8, 13}, {14, 16}, {17, 21}, {10, 15}, {12, 15}, {20, 26}, {0, 5}, {3, 6}, {5, 6}, {64, 70}, {65, 72}, {67, 75}, {64, 72}, {63, 69}, {11, 15}, {8, 11}, {8, 11}, {7, 9}, {10, 13}, {2, 10}}},
    {{{44, 52}, {47, 59}, {40, 48}, {43, 51}, {73, 83}, {}, {}, {73, 84}, {66, 73}, {63, 72}, {58, 72}, {}, {}, {}, {}, {}, {}, {}, {76, 83}, {89, 97}, {81, 90}, {76, 83}, {74, 82}, {}, {}, {}, {}, {65, 76}, {59, 69}, {56, 64}, {42, 47}, {49, 56}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {71, 79}, {73, 84}, {76, 88}, {73, 77}, {70, 81}, {}, {}, {}, {}, {}, {}}},
    {{{53, 60}, {60, 67}, {49, 57}, {52, 60}, {84, 90}, {}, {}, {85, 93}, {74, 79}, {73, 80}, {73, 79}, {}, {}, {}, {}, {}, {}, {}, {84, 91}, {}, {91, 96}, {}, {83, 88}, {}, {}, {}, {}, {77, 81}, {70, 76}, {65, 70}, {48, 51}, {57, 63}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {80, 87}, {85, 90}, {89, 95}, {78, 85}, {82, 87}, {16, 19}, {12, 15}, {12, 17}, {10, 13}, {14, 17}, {}}},
    {{{61, 70}, {68, 76}, {58, 63}, {61, 66}, {91, 98}, {45, 48}, {85, 94}, {94, 103}, {80, 87}, {81, 87}, {80, 87}, {}, {116, 131}, {125, 133}, {129, 139}, {122, 129}, {133, 140}, {}, {92, 99}, {98, 103}, {97, 101}, {84, 90}, {89, 96}, {20, 22}, {}, {}, {41, 54}, {82, 88}, {77, 84}, {71, 77}, {52, 54}, {64, 66}, {42, 50}, {17, 21}, {22, 27}, {16, 19}, {16, 18}, {}, {}, {}, {26, 36}, {88, 92}, {91, 95}, {96, 103}, {86, 89}, {88, 94}, {20, 23}, {16, 19}, {18, 24}, {14, 17}, {18, 21}, {}}},
    {{{71, 76}, {77, 84}, {64, 69}, {67, 75}, {99, 108}, {}, {}, {110, 113}, {92, 95}, {88, 90}, {88, 93}, {}, {144, 148}, {149, 152}, {156, 160}, {145, 149}, {150, 154}, {}, {109, 117}, {110, 117}, {111, 117}, {98, 107}, {106, 113}, {}, {}, {}, {}, {89, 96}, {91, 95}, {83, 92}, {61, 66}, {76, 79}, {}, {26, 29}, {31, 33}, {24, 26}, {22, 24}, {}, {}, {}, {}, {101, 110}, {105, 109}, {113, 122}, {97, 99}, {104, 110}, {24, 26}, {20, 21}, {25, 27}, {18, 20}, {22, 24}, {}}},
    {{{77, 83}, {85, 87}, {70, 73}, {76, 80}, {109, 112}, {}, {95, 102}, {120, 125}, {102, 106}, {91, 95}, {94, 98}, {}, {163, 167}, {177, 181}, {177, 181}, {165, 169}, {170, 174}, {}, {127, 131}, {}, {127, 132}, {}, {123, 126}, {}, {}, {}, {}, {97, 101}, {101, 104}, {99, 102}, {73, 76}, {87, 89}, {}, {34, 37}, {38, 40}, {31, 33}, {29, 32}, {70, 72}, {27, 29}, {}, {}, {122, 124}, {119, 124}, {133, 136}, {109, 114}, {120, 125}, {27, 30}, {22, 24}, {28, 30}, {21, 25}, {25, 28}, {39, 40}}},
    {{{84, 86}, {88, 91}, {74, 77}, {81, 84}, {113, 116}, {}, {103, 105}, {126, 131}, {107, 111}, {96, 101}, {99, 105}, {}, {168, 171}, {182, 185}, {182, 185}, {170, 173}, {175, 179}, {}, {132, 137}, {126, 129}, {133, 137}, {116, 121}, {127, 131}, {}, {}, {}, {}, {102, 108}, {105, 110}, {103, 107}, {77, 80}, {90, 92}, {51, 54}, {}, {}, {}, {33, 35}, {}, {}, {}, {37, 39}, {125, 129}, {125, 129}, {137, 140}, {115, 119}, {126, 129}, {}, {}, {}, {}, {}, {}}},
    {{{87, 91}, {92, 99}, {78, 81}, {85, 90}, {117, 121}, {}, {106, 110}, {132, 141}, {112, 116}, {102, 109}, {106, 110}, {}, {172, 179}, {186, 193}, {186, 193}, {174, 180}, {180, 187}, {26, 27}, {138, 145}, {130, 137}, {138, 146}, {122, 126}, {132, 137}, {}, {7, 10}, {}, {55, 57}, {109, 114}, {111, 117}, {108, 111}, {81, 84}, {93, 99}, {55, 60}, {38, 41}, {41, 44}, {34, 38}, {36, 40}, {}, {}, {}, {40, 41}, {130, 135}, {130, 136}, {141, 148}, {120, 127}, {130, 135}, {31, 35}, {25, 29}, {31, 35}, {26, 29}, {29, 33}, {41, 46}}},
    {{{92, 94}, {100, 104}, {82, 83}, {91, 95}, {122, 125}, {49, 51}, {111, 114}, {142, 145}, {117, 119}, {110, 112}, {111, 113}, {}, {180, 184}, {194, 199}, {194, 198}, {181, 183}, {188, 192}, {28, 29}, {146, 149}, {138, 140}, {147, 150}, {127, 131}, {138, 141}, {}, {11, 13}, {11, 14}, {58, 61}, {115, 119}, {118, 120}, {112, 113}, {85, 87}, {100, 103}, {61, 64}, {42, 44}, {45, 47}, {39, 41}, {41, 43}, {}, {30, 32}, {19, 22}, {42, 44}, {136, 138}, {137, 140}, {149, 152}, {128, 131}, {136, 139}, {36, 37}, {30, 32}, {36, 38}, {30, 32}, {34, 36}, {47, 49}}},
    {{{95, 99}, {105, 109}, {84, 88}, {96, 101}, {126, 129}, {}, {115, 118}, {146, 150}, {120, 124}, {113, 115}, {114, 116}, {}, {185, 188}, {200, 204}, {199, 202}, {184, 187}, {193, 196}, {}, {150, 153}, {141, 144}, {151, 154}, {132, 135}, {142, 145}, {}, {}, {}, {62, 64}, {120, 123}, {121, 124}, {114, 117}, {88, 89}, {104, 107}, {65, 68}, {45, 47}, {48, 50}, {42, 43}, {44, 46}, {}, {}, {23, 26}, {}, {139, 143}, {141, 144}, {153, 155}, {132, 135}, {140, 143}, {38, 39}, {33, 35}, {39, 41}, {33, 35}, {37, 39}, {}}},
    {{{100, 107}, {110, 118}, {89, 95}, {102, 109}, {130, 137}, {52, 55}, {119, 122}, {151, 159}, {125, 131}, {116, 125}, {117, 123}, {20, 22}, {189, 194}, {205, 210}, {203, 210}, {188, 192}, {197, 202}, {30, 32}, {154, 158}, {145, 149}, {155, 163}, {136, 142}, {146, 151}, {23, 28}, {14, 17}, {15, 18}, {65, 68}, {124, 130}, {125, 132}, {118, 122}, {90, 93}, {108, 113}, {69, 72}, {48, 51}, {51, 55}, {}, {47, 54}, {73, 78}, {33, 35}, {27, 31}, {45, 50}, {144, 150}, {145, 152}, {156, 162}, {136, 141}, {144, 151}, {40, 44}, {36, 39}, {42, 48}, {36, 41}, {40, 43}, {50, 53}}},
    {{{108, 117}, {119, 133}, {96, 104}, {110, 118}, {138, 152}, {}, {}, {160, 171}, {132, 143}, {126, 137}, {124, 137}, {}, {}, {}, {}, {}, {}, {}, {159, 169}, {150, 163}, {164, 173}, {143, 150}, {152, 158}, {}, {}, {}, {}, {131, 141}, {133, 144}, {123, 132}, {94, 99}, {114, 121}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {151, 160}, {153, 164}, {163, 171}, {142, 148}, {152, 165}, {}, {}, {}, {}, {}, {}}},
    {{{118, 123}, {138, 142}, {113, 117}, {119, 124}, {159, 163}, {}, {123, 129}, {175, 179}, {147, 152}, {141, 144}, {141, 148}, {}, {207, 211}, {225, 229}, {225, 229}, {206, 210}, {217, 221}, {}, {174, 179}, {}, {177, 181}, {}, {162, 166}, {}, {}, {}, {}, {144, 149}, {149, 150}, {137, 141}, {102, 104}, {125, 129}, {}, {52, 56}, {56, 59}, {44, 46}, {55, 59}, {79, 82}, {36, 38}, {}, {}, {163, 168}, {168, 172}, {176, 180}, {152, 157}, {169, 174}, {45, 48}, {40, 45}, {49, 51}, {42, 46}, {44, 47}, {}}},
    {{{124, 133}, {143, 154}, {118, 124}, {125, 131}, {164, 170}, {}, {}, {180, 190}, {153, 163}, {145, 154}, {149, 164}, {}, {}, {}, {}, {}, {}, {}, {180, 189}, {168, 180}, {182, 191}, {153, 162}, {167, 173}, {}, {}, {}, {}, {150, 163}, {151, 163}, {142, 152}, {105, 111}, {130, 137}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {169, 179}, {173, 187}, {181, 189}, {158, 170}, {175, 188}, {}, {}, {}, {}, {}, {}}},
    {{{134, 137}, {155, 158}, {125, 127}, {132, 135}, {171, 174}, {}, {}, {191, 195}, {164, 168}, {155, 158}, {165, 168}, {}, {212, 215}, {230, 234}, {230, 235}, {211, 215}, {222, 225}, {}, {190, 196}, {}, {192, 195}, {163, 164}, {174, 177}, {}, {}, {}, {}, {164, 168}, {164, 169}, {153, 155}, {112, 113}, {138, 140}, {}, {}, {60, 61}, {}, {60, 61}, {}, {}, {}, {}, {180, 181}, {188, 191}, {190, 193}, {171, 175}, {189, 193}, {}, {}, {}, {}, {48, 49}, {}}},
    {{{138, 141}, {159, 162}, {128, 131}, {136, 139}, {175, 178}, {}, {}, {}, {}, {}, {}, {}, {}, {235, 239}, {236, 241}, {216, 219}, {226, 229}, {}, {}, {}, {196, 199}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{142, 155}, {163, 173}, {132, 146}, {140, 153}, {179, 191}, {56, 59}, {130, 139}, {196, 206}, {169, 179}, {159, 165}, {169, 177}, {23, 27}, {216, 225}, {240, 246}, {242, 249}, {220, 231}, {230, 237}, {}, {197, 207}, {185, 193}, {200, 210}, {165, 173}, {178, 187}, {29, 33}, {18, 23}, {19, 22}, {69, 75}, {169, 177}, {170, 178}, {156, 162}, {114, 119}, {141, 148}, {73, 79}, {59, 64}, {62, 68}, {49, 53}, {62, 66}, {83, 91}, {39, 47}, {32, 37}, {51, 56}, {182, 189}, {192, 200}, {194, 198}, {176, 185}, {194, 202}, {51, 54}, {48, 53}, {54, 59}, {49, 54}, {50, 54}, {54, 60}}},
    {{{156, 163}, {174, 185}, {147, 150}, {154, 162}, {192, 201}, {}, {}, {207, 216}, {180, 186}, {166, 175}, {178, 191}, {}, {}, {}, {}, {}, {}, {}, {208, 217}, {194, 204}, {211, 220}, {174, 182}, {188, 193}, {}, {}, {}, {}, {178, 191}, {179, 185}, {163, 169}, {120, 126}, {149, 155}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {190, 198}, {201, 212}, {199, 208}, {186, 195}, {203, 214}, {}, {}, {}, {}, {}, {}}},
    {{{164, 175}, {186, 201}, {151, 158}, {163, 170}, {202, 213}, {60, 61}, {140, 144}, {217, 223}, {187, 194}, {176, 183}, {}, {}, {245, 250}, {267, 271}, {268, 275}, {251, 261}, {256, 263}, {33, 35}, {218, 226}, {205, 209}, {221, 230}, {183, 191}, {194, 199}, {}, {}, {23, 24}, {76, 80}, {192, 198}, {186, 192}, {170, 177}, {127, 131}, {}, {80, 83}, {65, 69}, {69, 75}, {54, 57}, {67, 70}, {92, 94}, {}, {38, 42}, {57, 59}, {199, 205}, {213, 221}, {209, 217}, {196, 204}, {215, 222}, {55, 59}, {54, 57}, {60, 64}, {55, 57}, {55, 59}, {}}},
    {{{176, 185}, {209, 219}, {159, 169}, {171, 180}, {214, 221}, {62, 64}, {145, 150}, {224, 233}, {195, 205}, {184, 192}, {197, 205}, {}, {251, 258}, {272, 279}, {276, 285}, {262, 270}, {264, 273}, {36, 37}, {227, 235}, {210, 218}, {231, 240}, {192, 204}, {200, 207}, {37, 39}, {24, 26}, {25, 27}, {81, 85}, {199, 208}, {193, 202}, {178, 183}, {132, 137}, {156, 163}, {84, 86}, {70, 75}, {76, 81}, {58, 61}, {71, 75}, {95, 97}, {48, 49}, {43, 46}, {64, 67}, {206, 211}, {222, 234}, {218, 224}, {205, 213}, {223, 233}, {60, 63}, {58, 61}, {65, 68}, {58, 61}, {60, 63}, {61, 65}}},
    {{{186, 192}, {220, 225}, {170, 175}, {181, 189}, {222, 228}, {}, {151, 156}, {234, 241}, {206, 213}, {193, 200}, {206, 214}, {31, 33}, {259, 264}, {280, 285}, {286, 291}, {271, 277}, {274, 283}, {38, 39}, {236, 243}, {219, 226}, {241, 250}, {205, 212}, {208, 215}, {40, 42}, {}, {28, 29}, {86, 90}, {209, 215}, {203, 210}, {184, 186}, {138, 141}, {164, 172}, {87, 89}, {76, 79}, {82, 85}, {62, 65}, {76, 78}, {}, {50, 51}, {47, 48}, {68, 70}, {212, 218}, {235, 241}, {225, 231}, {214, 219}, {234, 242}, {64, 66}, {62, 64}, {69, 72}, {62, 65}, {64, 66}, {66, 68}}},
    {{{193, 194}, {226, 226}, {}, {190, 191}, {229, 231}, {}, {157, 158}, {242, 245}, {214, 215}, {}, {}, {}, {}, {}, {}, {}, {284, 286}, {}, {244, 248}, {227, 228}, {251, 254}, {}, {216, 219}, {}, {}, {}, {91, 92}, {}, {211, 212}, {187, 189}, {142, 142}, {173, 173}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {242, 242}, {}, {220, 221}, {243, 244}, {}, {}, {}, {66, 66}, {}, {}}},
    {{{195, 198}, {227, 231}, {176, 183}, {192, 198}, {232, 237}, {65, 66}, {159, 162}, {246, 247}, {216, 219}, {201, 205}, {215, 218}, {}, {265, 269}, {286, 291}, {292, 297}, {278, 283}, {287, 290}, {40, 41}, {249, 249}, {229, 231}, {}, {213, 217}, {220, 220}, {}, {}, {}, {93, 97}, {216, 220}, {213, 216}, {}, {143, 144}, {174, 176}, {90, 93}, {80, 83}, {86, 89}, {66, 67}, {79, 81}, {98, 102}, {52, 54}, {49, 50}, {71, 72}, {219, 223}, {243, 246}, {232, 236}, {222, 225}, {245, 247}, {67, 70}, {65, 69}, {73, 74}, {67, 67}, {67, 70}, {69, 72}}},
    {{{199, 202}, {232, 237}, {184, 187}, {199, 204}, {238, 243}, {}, {}, {254, 258}, {225, 229}, {211, 213}, {226, 230}, {}, {286, 290}, {306, 310}, {313, 320}, {298, 301}, {307, 313}, {}, {255, 260}, {}, {260, 264}, {}, {227, 230}, {}, {}, {}, {}, {228, 232}, {224, 228}, {192, 196}, {149, 151}, {181, 185}, {}, {88, 91}, {94, 96}, {72, 74}, {86, 89}, {103, 104}, {}, {}, {}, {230, 235}, {252, 257}, {243, 247}, {231, 236}, {252, 256}, {74, 77}, {72, 75}, {77, 79}, {70, 73}, {75, 78}, {}}},
    {{{203, 205}, {238, 238}, {188, 191}, {205, 207}, {244, 245}, {}, {163, 165}, {259, 260}, {230, 231}, {214, 215}, {231, 232}, {}, {291, 293}, {311, 312}, {321, 323}, {302, 303}, {314, 316}, {}, {261, 262}, {236, 237}, {265, 266}, {223, 223}, {231, 232}, {}, {}, {}, {}, {233, 234}, {229, 230}, {197, 198}, {152, 153}, {186, 187}, {94, 95}, {92, 93}, {97, 98}, {75, 76}, {90, 91}, {}, {}, {}, {73, 74}, {236, 237}, {258, 259}, {248, 249}, {237, 238}, {257, 258}, {78, 79}, {76, 77}, {80, 81}, {74, 75}, {79, 79}, {}}},
    {{{206, 212}, {239, 241}, {}, {}, {246, 248}, {}, {}, {}, {}, {}, {}, {}, {}, {313, 314}, {324, 327}, {304, 307}, {317, 320}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{213, 221}, {242, 251}, {192, 200}, {208, 213}, {249, 255}, {}, {}, {294, 306}, {265, 273}, {254, 258}, {268, 278}, {}, {}, {}, {}, {}, {}, {}, {297, 308}, {269, 276}, {299, 308}, {260, 265}, {269, 272}, {}, {}, {}, {}, {277, 284}, {267, 274}, {229, 236}, {154, 158}, {218, 225}, {}, {}, {}, {}, {}, {}, {55, 59}, {}, {}, {271, 279}, {291, 301}, {285, 294}, {272, 281}, {294, 305}, {119, 123}, {114, 116}, {116, 120}, {112, 114}, {115, 117}, {}}},
    {{{222, 224}, {252, 254}, {201, 202}, {214, 216}, {256, 257}, {}, {}, {307, 308}, {274, 275}, {259, 260}, {279, 280}, {}, {304, 305}, {325, 326}, {332, 333}, {316, 318}, {333, 335}, {}, {309, 310}, {277, 278}, {309, 310}, {266, 267}, {273, 274}, {}, {}, {}, {}, {285, 286}, {}, {237, 239}, {159, 160}, {226, 227}, {}, {126, 127}, {132, 133}, {119, 120}, {131, 132}, {}, {}, {}, {}, {280, 281}, {302, 303}, {295, 297}, {282, 282}, {306, 307}, {124, 125}, {117, 117}, {121, 122}, {115, 116}, {118, 119}, {}}},
    {{{225, 231}, {255, 259}, {203, 208}, {217, 222}, {258, 263}, {}, {}, {309, 314}, {276, 280}, {261, 266}, {281, 285}, {}, {}, {}, {}, {}, {}, {}, {311, 316}, {}, {311, 315}, {}, {275, 279}, {}, {}, {}, {}, {287, 293}, {275, 280}, {240, 244}, {161, 164}, {228, 229}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {282, 287}, {304, 309}, {298, 302}, {283, 288}, {308, 312}, {126, 127}, {118, 120}, {123, 126}, {117, 120}, {120, 122}, {}}},
    {{{232, 234}, {260, 263}, {209, 212}, {223, 224}, {264, 265}, {}, {166, 167}, {315, 316}, {281, 282}, {267, 268}, {286, 287}, {}, {306, 309}, {327, 330}, {334, 338}, {319, 320}, {336, 338}, {}, {317, 320}, {279, 280}, {316, 317}, {268, 269}, {280, 281}, {}, {}, {}, {98, 100}, {294, 294}, {281, 283}, {245, 246}, {}, {230, 231}, {96, 97}, {128, 129}, {134, 135}, {121, 122}, {133, 134}, {105, 106}, {}, {51, 52}, {75, 76}, {288, 289}, {310, 312}, {303, 304}, {289, 291}, {313, 315}, {128, 128}, {121, 122}, {127, 128}, {121, 122}, {123, 124}, {73, 74}}},
    {{{235, 240}, {264, 275}, {213, 222}, {225, 230}, {266, 281}, {}, {}, {317, 327}, {283, 292}, {269, 282}, {288, 302}, {}, {}, {}, {}, {}, {}, {}, {321, 332}, {}, {318, 327}, {}, {282, 286}, {}, {}, {}, {}, {295, 305}, {284, 295}, {247, 254}, {165, 170}, {232, 240}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {290, 299}, {313, 323}, {305, 314}, {292, 299}, {316, 327}, {}, {}, {}, {}, {}, {}}},
    {{{241, 246}, {276, 284}, {223, 229}, {231, 239}, {282, 292}, {75, 78}, {174, 183}, {328, 335}, {293, 300}, {283, 287}, {303, 311}, {}, {319, 324}, {339, 346}, {351, 358}, {331, 335}, {349, 357}, {}, {333, 341}, {281, 288}, {328, 337}, {270, 276}, {287, 293}, {}, {}, {}, {}, {306, 317}, {296, 305}, {255, 262}, {171, 174}, {241, 245}, {}, {130, 137}, {136, 143}, {123, 129}, {135, 142}, {116, 124}, {60, 67}, {}, {}, {300, 307}, {324, 334}, {315, 323}, {300, 310}, {328, 334}, {129, 139}, {123, 127}, {129, 136}, {123, 129}, {125, 132}, {82, 90}}},
    {{{247, 251}, {285, 289}, {230, 232}, {240, 245}, {293, 295}, {}, {}, {336, 338}, {301, 303}, {288, 291}, {312, 315}, {}, {337, 340}, {359, 362}, {373, 377}, {355, 359}, {378, 382}, {}, {342, 345}, {289, 291}, {338, 341}, {277, 279}, {294, 296}, {}, {}, {}, {}, {318, 321}, {306, 309}, {263, 264}, {175, 177}, {246, 248}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {308, 310}, {335, 337}, {324, 326}, {311, 313}, {335, 337}, {}, {}, {}, {}, {}, {}}},
    {{{252, 256}, {290, 294}, {233, 236}, {246, 250}, {296, 298}, {}, {}, {339, 341}, {304, 306}, {292, 295}, {316, 319}, {}, {341, 344}, {363, 366}, {378, 382}, {360, 363}, {383, 387}, {}, {346, 349}, {292, 294}, {342, 345}, {280, 282}, {297, 299}, {}, {}, {}, {}, {322, 325}, {310, 312}, {265, 266}, {178, 180}, {249, 251}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {311, 313}, {338, 340}, {327, 329}, {314, 316}, {338, 340}, {}, {}, {}, {}, {}, {}}},
    {{{257, 259}, {}, {237, 238}, {255, 256}, {}, {}, {}, {345, 346}, {311, 312}, {296, 297}, {320, 321}, {}, {}, {}, {}, {}, {}, {}, {354, 355}, {299, 300}, {350, 351}, {283, 284}, {304, 305}, {}, {}, {}, {}, {326, 328}, {317, 318}, {267, 268}, {181, 182}, {255, 256}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {318, 319}, {345, 346}, {333, 333}, {321, 322}, {345, 346}, {}, {}, {}, {}, {}, {}}},
    {{{260, 270}, {298, 310}, {239, 244}, {257, 265}, {299, 307}, {}, {}, {347, 356}, {313, 323}, {298, 306}, {322, 334}, {}, {}, {}, {}, {}, {}, {}, {356, 365}, {301, 305}, {352, 363}, {285, 296}, {306, 315}, {}, {}, {}, {}, {329, 343}, {319, 330}, {269, 277}, {183, 188}, {257, 262}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {320, 325}, {347, 358}, {334, 343}, {323, 327}, {347, 358}, {}, {}, {}, {}, {}, {}}},
    {{{271, 275}, {311, 315}, {245, 249}, {266, 270}, {308, 313}, {81, 82}, {}, {357, 362}, {324, 329}, {307, 312}, {335, 338}, {}, {345, 349}, {371, 376}, {389, 394}, {368, 373}, {393, 397}, {}, {366, 371}, {306, 311}, {364, 368}, {297, 300}, {316, 322}, {}, {}, {}, {}, {344, 348}, {331, 336}, {278, 282}, {189, 192}, {263, 267}, {}, {138, 141}, {148, 150}, {130, 132}, {143, 145}, {125, 126}, {68, 70}, {}, {}, {326, 330}, {359, 364}, {344, 348}, {328, 333}, {359, 363}, {140, 141}, {133, 135}, {143, 145}, {134, 135}, {133, 135}, {}}},
    {{{276, 283}, {316, 328}, {250, 258}, {271, 278}, {314, 325}, {}, {}, {363, 373}, {330, 339}, {313, 324}, {339, 350}, {}, {}, {}, {}, {}, {}, {}, {372, 378}, {312, 322}, {369, 380}, {301, 312}, {323, 327}, {}, {}, {}, {}, {349, 362}, {337, 347}, {283, 290}, {193, 199}, {268, 276}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {331, 337}, {365, 377}, {349, 361}, {334, 345}, {364, 377}, {}, {}, {}, {}, {}, {}}},
    {{{284, 293}, {329, 340}, {259, 267}, {279, 288}, {326, 338}, {}, {}, {374, 383}, {340, 349}, {325, 334}, {351, 354}, {}, {}, {}, {}, {}, {}, {}, {379, 387}, {323, 333}, {381, 391}, {313, 322}, {328, 335}, {}, {}, {}, {}, {363, 371}, {348, 356}, {291, 298}, {}, {277, 283}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {338, 348}, {378, 386}, {362, 372}, {346, 352}, {378, 388}, {}, {}, {}, {}, {}, {}}},
    {{{}, {134, 137}, {105, 112}, {}, {153, 158}, {}, {}, {172, 174}, {144, 146}, {138, 140}, {138, 140}, {}, {203, 206}, {221, 224}, {221, 224}, {202, 205}, {213, 216}, {}, {170, 173}, {164, 167}, {174, 176}, {151, 152}, {159, 161}, {}, {}, {}, {}, {142, 143}, {145, 148}, {133, 136}, {100, 101}, {122, 124}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {161, 162}, {165, 167}, {172, 175}, {149, 151}, {166, 168}, {}, {}, {}, {}, {}, {}}},
    {{{}, {202, 208}, {}, {}, {}, {}, {}, {}, {}, {}, {192, 196}, {28, 30}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {34, 36}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {60, 63}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {295, 297}, {}, {251, 254}, {}, {79, 80}, {}, {342, 344}, {307, 310}, {}, {}, {}, {}, {367, 370}, {383, 388}, {364, 367}, {388, 392}, {}, {350, 353}, {295, 298}, {346, 349}, {}, {300, 303}, {}, {}, {}, {}, {}, {313, 316}, {}, {}, {252, 254}, {}, {}, {144, 147}, {}, {}, {}, {}, {}, {}, {314, 317}, {341, 344}, {330, 332}, {317, 320}, {341, 344}, {}, {128, 132}, {137, 142}, {130, 133}, {}, {}}},
    {{{}, {}, {}, {}, {0, 10}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {10, 22}, {11, 26}, {9, 18}, {11, 22}, {7, 16}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {8, 15}, {8, 13}, {6, 13}, {8, 17}, {9, 16}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {11, 17}, {3, 4}, {4, 8}, {}, {}, {}, {}, {}, {10, 15}, {8, 12}, {8, 12}, {5, 9}, {5, 9}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {16, 25}, {14, 22}, {14, 24}, {18, 24}, {17, 23}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {18, 24}, {5, 7}, {}, {}, {}, {}, {}, {}, {16, 21}, {13, 17}, {13, 17}, {10, 14}, {10, 14}, {}, {23, 32}, {27, 38}, {19, 28}, {23, 31}, {17, 26}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {0, 2}, {0, 3}, {}, {}, {}, {}, {}, {0, 9}, {0, 7}, {0, 7}, {0, 4}, {0, 4}, {}, {0, 9}, {0, 10}, {0, 8}, {0, 10}, {0, 6}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 7}, {}, {}, {}, {0, 7}, {0, 7}, {0, 5}, {0, 7}, {0, 8}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {8, 11}, {13, 20}, {}, {}, {}, {}, {}, {26, 31}, {22, 26}, {22, 27}, {19, 23}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {8, 13}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {12, 15}, {}, {}, {}, {}, {}, {}, {}, {27, 34}, {28, 36}, {24, 28}, {19, 23}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {14, 19}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {22, 25}, {}, {}, {}, {}, {}, {}, {60, 65}, {71, 78}, {72, 76}, {67, 74}, {67, 72}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {27, 34}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {26, 29}, {35, 43}, {}, {}, {}, {}, {}, {76, 79}, {87, 90}, {86, 91}, {83, 87}, {81, 86}, {8, 10}, {}, {}, {}, {}, {}, {}, {}, {}, {11, 16}, {}, {}, {}, {}, {}, {14, 17}, {}, {}, {}, {}, {42, 45}, {6, 10}, {}, {7, 8}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {11, 14}}},
    {{{}, {}, {}, {}, {}, {30, 31}, {44, 48}, {}, {}, {}, {}, {4, 7}, {80, 83}, {91, 94}, {92, 95}, {88, 91}, {87, 92}, {11, 12}, {}, {}, {}, {}, {}, {0, 1}, {}, {4, 5}, {17, 19}, {}, {}, {}, {}, {}, {18, 20}, {}, {}, {}, {}, {46, 47}, {11, 13}, {7, 9}, {9, 11}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {15, 18}}},
    {{{}, {}, {}, {}, {}, {32, 34}, {49, 52}, {}, {}, {}, {}, {8, 10}, {84, 87}, {95, 98}, {96, 99}, {92, 95}, {93, 98}, {13, 15}, {}, {}, {}, {}, {}, {2, 6}, {}, {}, {20, 23}, {}, {}, {}, {}, {}, {21, 22}, {}, {}, {}, {}, {48, 51}, {14, 16}, {10, 12}, {12, 15}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {19, 21}}},
    {{{}, {}, {}, {}, {}, {35, 38}, {53, 60}, {}, {}, {}, {}, {11, 13}, {92, 97}, {103, 108}, {104, 109}, {100, 105}, {105, 110}, {16, 20}, {}, {}, {}, {}, {}, {7, 16}, {}, {6, 8}, {24, 31}, {}, {}, {}, {}, {}, {23, 29}, {}, {}, {}, {}, {52, 55}, {17, 20}, {}, {16, 19}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {22, 29}}},
    {{{}, {}, {}, {}, {}, {39, 39}, {61, 68}, {}, {}, {}, {}, {14, 16}, {98, 102}, {109, 112}, {110, 114}, {106, 109}, {111, 116}, {21, 22}, {}, {}, {}, {}, {}, {17, 19}, {}, {9, 10}, {32, 34}, {}, {}, {}, {}, {}, {30, 36}, {}, {}, {}, {}, {56, 58}, {21, 23}, {13, 14}, {20, 22}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {30, 32}}},
    {{{}, {}, {}, {}, {}, {40, 42}, {69, 77}, {}, {}, {}, {}, {}, {103, 107}, {113, 117}, {115, 119}, {110, 114}, {117, 121}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {59, 63}, {24, 26}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {33, 34}}},
    {{{}, {}, {}, {}, {}, {43, 44}, {}, {}, {}, {}, {}, {17, 19}, {113, 115}, {122, 124}, {126, 128}, {119, 121}, {128, 132}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {68, 69}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {67, 70}, {}, {282, 284}, {253, 255}, {242, 245}, {258, 260}, {}, {299, 303}, {320, 324}, {}, {312, 315}, {327, 332}, {}, {284, 286}, {259, 261}, {288, 290}, {246, 248}, {256, 258}, {}, {}, {}, {}, {263, 266}, {253, 255}, {217, 217}, {}, {207, 208}, {}, {112, 115}, {117, 120}, {103, 106}, {119, 121}, {}, {}, {}, {}, {259, 261}, {280, 282}, {272, 274}, {261, 263}, {281, 283}, {103, 106}, {100, 103}, {106, 109}, {100, 103}, {102, 103}, {}}},
    {{{}, {}, {}, {}, {}, {71, 74}, {168, 173}, {}, {}, {}, {}, {}, {310, 318}, {331, 338}, {339, 350}, {321, 330}, {339, 348}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {107, 115}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {78, 84}, {}, {}, {}, {}, {}, {108, 112}, {118, 121}, {120, 125}, {115, 118}, {122, 127}, {23, 25}, {}, {}, {}, {}, {}, {}, {}, {}, {35, 40}, {}, {}, {}, {}, {}, {37, 41}, {}, {}, {}, {}, {64, 67}, {}, {15, 18}, {23, 25}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {35, 38}}},
    {{{}, {}, {}, {}, {}, {}, {184, 187}, {}, {}, {}, {}, {}, {325, 330}, {347, 352}, {359, 364}, {336, 344}, {358, 367}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {0, 8}, {0, 7}, {0, 7}, {0, 5}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {9, 19}, {8, 16}, {8, 16}, {6, 12}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {5, 6}, {8, 13}, {5, 9}, {5, 9}, {6, 11}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 2}, {0, 1}, {0, 2}, {}, {0, 2}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {38, 46}, {34, 39}, {32, 39}, {29, 36}, {}, {}, {}, {}, {}, {}, {}, {}, {56, 65}, {43, 51}, {47, 53}, {41, 47}, {}, {}, {}, {}, {31, 37}, {28, 35}, {28, 34}, {18, 23}, {20, 25}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {40, 48}, {42, 49}, {43, 50}, {37, 47}, {42, 46}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {104, 105}, {88, 89}, {}, {}, {}, {132, 135}, {134, 136}, {140, 143}, {}, {141, 143}, {}, {100, 102}, {104, 105}, {102, 104}, {91, 92}, {97, 99}, {}, {}, {}, {}, {}, {}, {78, 79}, {55, 56}, {67, 69}, {}, {}, {28, 30}, {20, 21}, {19, 20}, {}, {}, {}, {}, {93, 94}, {96, 98}, {104, 106}, {90, 90}, {95, 97}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {106, 107}, {90, 91}, {}, {}, {}, {136, 139}, {137, 139}, {}, {130, 134}, {}, {}, {103, 105}, {106, 107}, {105, 107}, {93, 95}, {100, 102}, {}, {}, {}, {}, {}, {85, 87}, {80, 81}, {57, 58}, {70, 72}, {}, {22, 25}, {}, {22, 23}, {21, 21}, {}, {}, {}, {}, {95, 97}, {99, 101}, {107, 109}, {91, 93}, {98, 100}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {108, 109}, {}, {}, {}, {}, {140, 143}, {143, 145}, {148, 151}, {140, 144}, {}, {}, {106, 108}, {108, 109}, {108, 110}, {96, 97}, {103, 105}, {}, {}, {}, {}, {}, {88, 90}, {82, 82}, {59, 60}, {73, 75}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {98, 100}, {102, 104}, {110, 112}, {94, 96}, {101, 103}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {114, 115}, {96, 98}, {}, {}, {}, {149, 153}, {153, 157}, {161, 164}, {}, {155, 159}, {}, {118, 120}, {118, 119}, {118, 120}, {108, 109}, {114, 116}, {}, {}, {}, {}, {}, {}, {93, 93}, {67, 68}, {80, 82}, {}, {}, {34, 37}, {27, 28}, {25, 26}, {}, {}, {}, {}, {111, 114}, {110, 112}, {123, 126}, {100, 102}, {111, 113}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {116, 117}, {99, 101}, {}, {}, {}, {154, 158}, {158, 162}, {}, {150, 154}, {}, {}, {121, 123}, {120, 123}, {121, 123}, {110, 112}, {117, 119}, {}, {}, {}, {}, {}, {96, 97}, {94, 96}, {69, 70}, {83, 84}, {}, {30, 33}, {}, {29, 30}, {27, 28}, {}, {}, {}, {}, {115, 118}, {113, 115}, {127, 129}, {103, 105}, {114, 116}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {118, 119}, {}, {}, {}, {}, {159, 162}, {168, 171}, {169, 172}, {160, 164}, {}, {}, {124, 126}, {124, 125}, {124, 126}, {113, 115}, {120, 122}, {}, {}, {}, {}, {}, {98, 100}, {97, 98}, {71, 72}, {85, 86}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {119, 121}, {116, 118}, {130, 132}, {106, 108}, {117, 119}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {248, 253}, {220, 224}, {206, 210}, {219, 225}, {}, {281, 285}, {301, 305}, {307, 312}, {293, 297}, {301, 306}, {}, {250, 254}, {232, 235}, {256, 259}, {218, 222}, {221, 226}, {}, {}, {}, {}, {221, 227}, {217, 223}, {190, 191}, {145, 148}, {177, 180}, {}, {84, 87}, {90, 93}, {68, 71}, {82, 85}, {}, {}, {}, {}, {224, 229}, {247, 251}, {237, 242}, {226, 230}, {248, 251}, {71, 73}, {70, 71}, {75, 76}, {68, 69}, {71, 74}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {261, 263}, {232, 234}, {216, 219}, {233, 236}, {}, {}, {}, {}, {}, {}, {}, {263, 265}, {238, 240}, {267, 269}, {224, 226}, {233, 235}, {}, {}, {}, {}, {235, 238}, {231, 233}, {199, 201}, {}, {188, 190}, {}, {94, 97}, {99, 102}, {77, 80}, {92, 95}, {}, {}, {}, {}, {238, 240}, {260, 262}, {250, 252}, {239, 241}, {259, 261}, {80, 83}, {78, 80}, {82, 85}, {76, 78}, {80, 83}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {264, 266}, {235, 237}, {220, 222}, {237, 240}, {}, {}, {}, {}, {}, {}, {}, {266, 268}, {241, 243}, {270, 272}, {227, 229}, {236, 238}, {}, {}, {}, {}, {239, 242}, {234, 236}, {202, 204}, {}, {191, 193}, {}, {98, 101}, {103, 106}, {81, 83}, {96, 99}, {}, {}, {}, {}, {241, 243}, {263, 265}, {253, 255}, {242, 244}, {262, 264}, {84, 87}, {81, 82}, {86, 89}, {79, 82}, {84, 87}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {267, 268}, {238, 240}, {223, 225}, {241, 243}, {}, {}, {}, {}, {}, {}, {}, {269, 271}, {244, 246}, {273, 275}, {230, 232}, {239, 242}, {}, {}, {}, {}, {243, 246}, {237, 239}, {205, 207}, {}, {194, 196}, {}, {102, 104}, {107, 109}, {84, 86}, {100, 103}, {}, {}, {}, {}, {244, 246}, {266, 268}, {256, 258}, {245, 248}, {265, 267}, {88, 91}, {83, 86}, {90, 92}, {83, 86}, {88, 90}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {269, 272}, {241, 243}, {226, 229}, {244, 247}, {}, {}, {}, {}, {}, {}, {}, {272, 274}, {247, 249}, {276, 278}, {233, 236}, {243, 246}, {}, {}, {}, {}, {247, 250}, {240, 241}, {208, 209}, {}, {197, 198}, {}, {105, 107}, {110, 112}, {87, 90}, {104, 107}, {}, {}, {}, {}, {247, 249}, {269, 270}, {259, 262}, {249, 251}, {268, 271}, {92, 95}, {87, 88}, {93, 94}, {87, 88}, {91, 92}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {273, 275}, {244, 246}, {230, 233}, {248, 251}, {}, {}, {}, {}, {}, {}, {}, {275, 277}, {250, 252}, {279, 281}, {237, 239}, {247, 249}, {}, {}, {}, {}, {251, 254}, {242, 245}, {210, 211}, {}, {199, 201}, {}, {}, {}, {91, 94}, {108, 110}, {}, {}, {}, {}, {250, 252}, {271, 273}, {263, 265}, {252, 254}, {272, 274}, {96, 98}, {89, 92}, {95, 97}, {89, 91}, {93, 95}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {276, 278}, {247, 249}, {234, 237}, {252, 254}, {}, {294, 298}, {315, 319}, {328, 331}, {308, 311}, {321, 326}, {}, {278, 280}, {253, 255}, {282, 284}, {240, 242}, {250, 252}, {}, {}, {}, {}, {255, 258}, {246, 249}, {212, 213}, {}, {202, 203}, {}, {108, 111}, {113, 116}, {95, 98}, {111, 114}, {}, {}, {}, {}, {253, 255}, {274, 276}, {266, 268}, {255, 257}, {275, 277}, {99, 102}, {93, 96}, {98, 101}, {92, 95}, {96, 98}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {279, 281}, {250, 252}, {238, 241}, {255, 257}, {}, {}, {}, {}, {}, {}, {}, {281, 283}, {256, 258}, {285, 287}, {243, 245}, {253, 255}, {}, {}, {}, {}, {259, 262}, {250, 252}, {214, 216}, {}, {204, 206}, {}, {}, {}, {99, 102}, {115, 118}, {}, {}, {}, {}, {256, 258}, {277, 279}, {269, 271}, {258, 260}, {278, 280}, {}, {97, 99}, {102, 105}, {96, 99}, {99, 101}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {285, 287}, {256, 259}, {246, 249}, {261, 263}, {}, {}, {}, {}, {}, {}, {}, {287, 290}, {262, 264}, {291, 293}, {249, 253}, {259, 262}, {}, {}, {}, {}, {267, 268}, {256, 259}, {218, 222}, {}, {209, 211}, {}, {116, 118}, {121, 124}, {107, 110}, {122, 125}, {}, {}, {}, {}, {262, 264}, {283, 286}, {275, 278}, {264, 265}, {284, 287}, {107, 110}, {104, 107}, {110, 111}, {104, 106}, {104, 106}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {288, 290}, {260, 261}, {250, 250}, {264, 265}, {}, {}, {}, {}, {}, {}, {}, {291, 293}, {265, 266}, {294, 295}, {254, 256}, {263, 265}, {}, {}, {}, {}, {269, 272}, {260, 262}, {223, 225}, {}, {212, 214}, {}, {119, 122}, {125, 127}, {111, 114}, {126, 127}, {}, {}, {}, {}, {265, 267}, {287, 287}, {279, 281}, {266, 268}, {288, 290}, {111, 114}, {108, 110}, {112, 113}, {107, 108}, {107, 110}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {291, 293}, {262, 264}, {251, 253}, {266, 267}, {}, {}, {}, {}, {}, {}, {}, {294, 296}, {267, 268}, {296, 298}, {257, 259}, {266, 268}, {}, {}, {}, {}, {273, 276}, {263, 266}, {226, 228}, {}, {215, 217}, {}, {123, 125}, {128, 131}, {115, 118}, {128, 130}, {}, {}, {}, {}, {268, 270}, {288, 290}, {282, 284}, {269, 271}, {291, 293}, {115, 118}, {111, 113}, {114, 115}, {109, 111}, {111, 114}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {34, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {66, 70}, {79, 82}, {77, 80}, {75, 78}, {73, 76}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {35, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {71, 75}, {83, 86}, {81, 85}, {79, 82}, {77, 80}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {39, 41}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {88, 91}, {99, 102}, {100, 103}, {96, 99}, {99, 104}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {195, 198}, {211, 216}, {211, 216}, {193, 197}, {203, 206}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {199, 202}, {217, 220}, {217, 220}, {198, 201}, {207, 212}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {226, 230}, {247, 251}, {250, 253}, {232, 236}, {238, 241}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {231, 235}, {252, 256}, {254, 257}, {237, 241}, {242, 246}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {236, 240}, {257, 262}, {258, 263}, {242, 246}, {247, 251}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {241, 244}, {263, 266}, {264, 267}, {247, 250}, {252, 255}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {270, 274}, {292, 295}, {298, 301}, {284, 287}, {291, 295}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {275, 280}, {296, 300}, {302, 306}, {288, 292}, {296, 300}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {331, 336}, {353, 358}, {365, 372}, {345, 354}, {368, 377}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {35, 39}, {37, 42}, {29, 34}, {24, 28}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {40, 44}, {43, 48}, {35, 39}, {29, 33}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {140, 142}, {144, 147}, {135, 139}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {146, 148}, {152, 155}, {}, {147, 149}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {163, 167}, {165, 168}, {155, 159}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {172, 176}, {173, 176}, {}, {165, 169}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {34, 38}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {144, 146}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {160, 164}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {181, 184}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {57, 58}, {}, {47, 48}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {49, 50}, {46, 47}, {52, 53}, {47, 48}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 4}, {0, 7}, {0, 4}, {0, 4}, {0, 5}, {}, {0, 1}, {0, 3}, {0, 2}, {0, 1}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {5, 8}, {7, 11}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {75, 81}}},
}};

// 0x8C42B0
static constexpr notsa::mdarray<tSpecificSpeechContextInfo, CTX_GFD_END, VOICE_GFD_END> gGfdSpeechLookup{{
    {{{0, 9}, {}, {}, {}, {4, 12}, {0, 8}, {0, 7}, {0, 6}, {0, 7}, {0, 5}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{10, 15}, {}, {}, {}, {}, {9, 15}, {8, 12}, {7, 13}, {8, 15}, {6, 13}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{16, 20}, {}, {}, {}, {}, {16, 21}, {13, 15}, {14, 17}, {16, 20}, {14, 18}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{21, 25}, {}, {}, {}, {13, 16}, {22, 27}, {16, 19}, {18, 20}, {21, 24}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{26, 30}, {}, {}, {}, {17, 20}, {28, 32}, {20, 23}, {21, 24}, {25, 29}, {19, 22}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{31, 37}, {}, {}, {}, {21, 28}, {33, 39}, {24, 29}, {25, 28}, {30, 37}, {23, 30}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{38, 41}, {}, {}, {}, {29, 34}, {40, 44}, {30, 33}, {29, 32}, {38, 42}, {31, 35}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{42, 46}, {}, {}, {}, {}, {45, 50}, {34, 38}, {33, 36}, {43, 48}, {41, 45}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{47, 51}, {}, {}, {}, {}, {51, 55}, {39, 42}, {37, 40}, {49, 53}, {46, 50}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{52, 57}, {}, {}, {}, {}, {56, 59}, {43, 47}, {41, 44}, {54, 58}, {51, 54}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{58, 63}, {}, {}, {}, {68, 72}, {60, 65}, {64, 68}, {45, 49}, {64, 68}, {59, 63}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{64, 69}, {}, {}, {}, {73, 84}, {66, 71}, {69, 76}, {50, 55}, {69, 75}, {64, 68}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{70, 74}, {}, {}, {}, {}, {72, 76}, {77, 81}, {56, 61}, {76, 80}, {69, 72}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{75, 76}, {}, {}, {}, {}, {77, 79}, {82, 83}, {62, 65}, {81, 83}, {73, 75}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{77, 80}, {}, {}, {}, {}, {80, 84}, {84, 87}, {66, 69}, {84, 88}, {76, 80}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{81, 85}, {}, {}, {}, {}, {85, 89}, {88, 92}, {70, 73}, {89, 92}, {81, 84}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{86, 87}, {}, {}, {}, {}, {90, 91}, {93, 98}, {74, 76}, {93, 95}, {85, 87}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{88, 89}, {}, {}, {}, {}, {92, 93}, {99, 102}, {77, 81}, {96, 99}, {88, 89}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{90, 92}, {}, {}, {}, {}, {94, 96}, {103, 105}, {82, 84}, {100, 103}, {90, 92}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{93, 94}, {}, {}, {}, {}, {97, 98}, {106, 107}, {85, 85}, {104, 105}, {93, 94}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{95, 100}, {}, {}, {}, {}, {99, 103}, {108, 112}, {86, 90}, {106, 110}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{101, 102}, {}, {}, {}, {}, {109, 111}, {113, 115}, {91, 92}, {116, 116}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{103, 107}, {}, {}, {}, {}, {112, 115}, {116, 120}, {93, 97}, {117, 121}, {95, 99}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{108, 112}, {}, {}, {}, {}, {116, 119}, {121, 124}, {98, 102}, {122, 126}, {100, 104}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{113, 117}, {}, {}, {}, {}, {120, 123}, {125, 129}, {103, 106}, {127, 130}, {105, 109}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{118, 120}, {}, {}, {}, {}, {124, 126}, {130, 132}, {107, 110}, {131, 133}, {110, 111}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{121, 124}, {}, {}, {}, {}, {127, 133}, {133, 136}, {111, 115}, {134, 137}, {112, 116}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{125, 128}, {}, {}, {}, {}, {134, 138}, {137, 139}, {116, 119}, {138, 143}, {117, 120}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{129, 134}, {}, {}, {}, {}, {139, 144}, {140, 145}, {120, 123}, {144, 149}, {121, 125}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{135, 144}, {}, {}, {}, {}, {145, 154}, {146, 155}, {124, 133}, {150, 159}, {126, 135}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{145, 149}, {}, {}, {}, {}, {155, 159}, {156, 159}, {134, 138}, {160, 165}, {136, 140}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{150, 152}, {}, {}, {}, {}, {160, 170}, {160, 162}, {139, 142}, {166, 168}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{153, 154}, {}, {}, {}, {}, {171, 172}, {163, 163}, {143, 143}, {169, 171}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{155, 156}, {}, {}, {}, {}, {173, 174}, {164, 165}, {144, 145}, {172, 173}, {141, 142}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{157, 158}, {}, {}, {}, {}, {175, 176}, {166, 168}, {146, 149}, {174, 175}, {143, 144}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{159, 164}, {}, {}, {}, {}, {177, 180}, {169, 172}, {150, 154}, {176, 182}, {145, 150}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{165, 169}, {}, {}, {}, {}, {181, 185}, {173, 176}, {155, 159}, {183, 186}, {151, 155}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{170, 174}, {}, {}, {}, {}, {186, 190}, {177, 181}, {160, 164}, {187, 190}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{175, 179}, {}, {}, {}, {}, {191, 194}, {182, 186}, {165, 168}, {191, 195}, {156, 160}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{180, 185}, {}, {}, {}, {}, {195, 199}, {187, 190}, {169, 173}, {196, 200}, {161, 166}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{186, 191}, {}, {}, {}, {}, {200, 205}, {191, 195}, {174, 181}, {201, 205}, {167, 171}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{192, 196}, {}, {}, {}, {}, {206, 211}, {196, 201}, {182, 188}, {206, 211}, {172, 178}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{197, 198}, {}, {}, {}, {}, {}, {202, 202}, {189, 189}, {212, 212}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{199, 203}, {}, {}, {}, {}, {212, 216}, {203, 206}, {190, 193}, {213, 216}, {179, 180}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{204, 208}, {}, {}, {}, {}, {217, 221}, {207, 210}, {194, 198}, {217, 221}, {181, 185}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{209, 213}, {}, {}, {}, {}, {222, 226}, {211, 214}, {199, 202}, {222, 226}, {186, 190}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{214, 215}, {}, {}, {}, {}, {}, {215, 215}, {203, 203}, {227, 227}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{216, 220}, {}, {}, {}, {}, {227, 231}, {216, 219}, {204, 207}, {228, 232}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{221, 223}, {}, {}, {}, {}, {232, 234}, {220, 222}, {208, 210}, {233, 235}, {191, 193}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{224, 228}, {}, {}, {}, {}, {235, 239}, {223, 225}, {211, 213}, {236, 239}, {194, 197}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{229, 231}, {}, {}, {}, {}, {}, {226, 226}, {214, 215}, {}, {198, 200}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{232, 235}, {}, {}, {}, {}, {}, {227, 228}, {216, 217}, {}, {201, 204}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{236, 238}, {}, {}, {}, {}, {240, 242}, {229, 231}, {218, 220}, {240, 243}, {205, 207}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{239, 243}, {}, {}, {}, {}, {243, 247}, {232, 234}, {221, 223}, {244, 246}, {208, 210}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{244, 247}, {}, {}, {}, {}, {248, 251}, {235, 237}, {224, 226}, {247, 250}, {211, 213}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{248, 251}, {}, {}, {}, {}, {252, 255}, {238, 240}, {227, 230}, {251, 255}, {214, 217}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{252, 254}, {}, {}, {}, {}, {256, 258}, {241, 243}, {231, 233}, {256, 257}, {218, 220}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{255, 259}, {}, {}, {}, {}, {259, 261}, {244, 245}, {234, 234}, {258, 260}, {221, 225}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{260, 264}, {}, {}, {}, {}, {262, 267}, {246, 250}, {235, 236}, {266, 269}, {226, 229}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{265, 268}, {}, {}, {}, {}, {268, 272}, {251, 254}, {237, 238}, {270, 276}, {230, 235}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{269, 273}, {}, {}, {}, {}, {273, 278}, {255, 259}, {239, 243}, {277, 283}, {236, 240}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{274, 287}, {}, {}, {}, {}, {284, 304}, {260, 276}, {244, 256}, {284, 295}, {248, 262}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{288, 292}, {}, {}, {}, {}, {305, 309}, {277, 281}, {257, 260}, {296, 300}, {263, 268}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{293, 297}, {}, {}, {}, {}, {310, 315}, {282, 286}, {261, 265}, {301, 304}, {269, 274}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{298, 306}, {}, {}, {}, {}, {316, 320}, {287, 291}, {}, {305, 310}, {275, 278}, {}, {10, 13}, {5, 9}, {}, {}, {12, 15}, {0, 2}, {15, 19}}},
    {{{307, 313}, {}, {}, {}, {85, 88}, {321, 325}, {292, 299}, {270, 275}, {311, 316}, {283, 288}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{314, 316}, {}, {}, {}, {89, 92}, {326, 329}, {300, 303}, {276, 277}, {317, 322}, {289, 293}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{317, 320}, {}, {}, {}, {}, {330, 334}, {304, 306}, {278, 281}, {323, 326}, {294, 297}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{321, 324}, {}, {}, {}, {}, {}, {307, 310}, {}, {327, 330}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{325, 328}, {}, {}, {}, {}, {}, {311, 314}, {}, {331, 334}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{329, 332}, {}, {}, {}, {}, {}, {315, 319}, {}, {335, 337}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{333, 335}, {}, {}, {}, {}, {}, {320, 324}, {}, {338, 340}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{336, 339}, {}, {}, {}, {}, {}, {325, 327}, {}, {341, 344}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{340, 343}, {}, {}, {}, {}, {}, {328, 330}, {}, {345, 348}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{344, 346}, {}, {}, {}, {}, {}, {331, 333}, {}, {349, 352}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{347, 349}, {}, {}, {}, {}, {}, {334, 337}, {}, {353, 356}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{350, 352}, {}, {}, {}, {}, {}, {338, 341}, {}, {357, 360}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{353, 355}, {}, {}, {}, {}, {}, {342, 344}, {}, {361, 362}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{356, 358}, {}, {}, {}, {}, {}, {345, 347}, {}, {363, 365}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{359, 363}, {}, {}, {}, {}, {}, {348, 352}, {282, 285}, {366, 367}, {298, 301}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{364, 365}, {}, {}, {}, {}, {335, 336}, {353, 354}, {286, 287}, {368, 369}, {302, 303}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{366, 370}, {}, {}, {}, {}, {337, 341}, {355, 359}, {288, 290}, {370, 374}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{371, 374}, {}, {}, {}, {}, {342, 344}, {360, 363}, {291, 293}, {375, 377}, {304, 307}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{375, 379}, {}, {}, {}, {}, {345, 348}, {367, 372}, {294, 298}, {378, 382}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {0, 7}, {0, 7}, {0, 9}, {}, {}, {}, {}, {}, {}, {}, {3, 9}, {0, 4}, {8, 17}, {0, 8}, {4, 11}, {}, {5, 14}}},
    {{{}, {8, 9}, {8, 9}, {10, 16}, {}, {}, {}, {}, {}, {}, {}, {14, 15}, {10, 11}, {18, 19}, {9, 10}, {16, 17}, {3, 4}, {}}},
    {{{}, {10, 13}, {10, 14}, {25, 29}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {20, 33}, {11, 14}, {}, {}, {}}},
    {{{}, {14, 19}, {15, 22}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {34, 52}, {15, 21}, {}, {}, {}}},
    {{{}, {20, 22}, {23, 25}, {}, {}, {}, {364, 366}, {}, {}, {}, {}, {23, 25}, {20, 21}, {53, 55}, {22, 24}, {27, 28}, {14, 15}, {}}},
    {{{}, {23, 28}, {26, 32}, {30, 39}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {25, 31}, {}, {}, {}}},
    {{{}, {29, 32}, {33, 36}, {40, 49}, {}, {}, {}, {}, {}, {}, {}, {26, 29}, {22, 26}, {56, 59}, {32, 35}, {}, {16, 18}, {}}},
    {{{}, {33, 40}, {37, 45}, {50, 59}, {}, {}, {}, {}, {}, {}, {}, {30, 33}, {}, {60, 74}, {36, 45}, {29, 37}, {}, {32, 41}}},
    {{{}, {}, {}, {17, 24}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {0, 3}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {35, 39}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {40, 45}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {46, 50}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {51, 58}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {59, 63}, {}, {}, {}, {59, 63}, {55, 58}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {64, 67}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {93, 98}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {104, 108}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {279, 283}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {48, 51}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {52, 55}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {56, 59}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {60, 63}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {266, 269}, {}, {279, 282}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {111, 115}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {261, 265}, {}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {36, 40}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {241, 247}, {}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 29}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {30, 81}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {82, 107}, {}, {}, {}, {}, {}, {}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {0, 2}, {}, {0, 7}, {}, {0, 3}, {}, {0, 4}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {16, 18}, {}, {}, {}, {18, 22}, {}, {20, 26}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {19, 22}, {12, 15}, {}, {}, {23, 26}, {5, 9}, {27, 31}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {16, 19}, {}, {}, {}, {10, 13}, {}}},
    {{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {27, 34}, {}, {}, {}, {19, 24}, {}}},
}};

// 0x8C64B8
static constexpr notsa::mdarray<tSpecificSpeechContextInfo, CTX_PAIN_END, VOICE_PAIN_END> gPainSpeechLookup{{
    // CARL       MALE        FEMALE
    {{{0, 13},    {},         {}       }},
    {{{14, 24},   {},         {}       }},
    {{{25, 33},   {},         {}       }},
    {{{34, 39},   {},         {}       }},
    {{{40, 63},   {},         {}       }},
    {{{64, 78},   {},         {}       }},
    {{{79, 86},   {},         {}       }},
    {{{87, 100},  {},         {}       }},
    {{{101, 121}, {},         {}       }},
    {{{122, 127}, {},         {}       }},
    {{{128, 133}, {},         {}       }},
    {{{134, 143}, {},         {}       }},
    {{{144, 155}, {},         {}       }},
    {{{156, 162}, {},         {}       }},
    {{{163, 168}, {},         {}       }},
    {{{},         {0, 24},    {0, 6}   }},
    {{{},         {25, 35},   {7, 18}  }},
    {{{},         {36, 45},   {19, 35} }},
    {{{},         {46, 87},   {36, 68} }},
    {{{},         {88, 102},  {69, 83} }},
    {{{},         {103, 119}, {84, 94} }},
    {{{},         {120, 130}, {95, 100}}},
}};
};

//
// SpeechVoice => SoundBank LUTs
//
namespace {
// 0x8AF700 (Originally int16)
static constexpr std::array<eSoundBank, VOICE_GEN_END> gGenSpeechVoiceToBankLookup = {
    SND_BANK_SPC_GA_BBDYG1,  // VOICE_GEN_BBDYG1
    SND_BANK_SPC_GA_BBDYG2,  // VOICE_GEN_BBDYG2
    SND_BANK_SPC_GA_BFORI,   // VOICE_GEN_BFORI
    SND_BANK_SPC_GA_BFOST,   // VOICE_GEN_BFOST
    SND_BANK_SPC_GA_BFYBE,   // VOICE_GEN_BFYBE
    SND_BANK_SPC_GA_BFYBU,   // VOICE_GEN_BFYBU
    SND_BANK_SPC_GA_BFYCRP,  // VOICE_GEN_BFYCRP
    SND_BANK_SPC_GA_BFYPRO,  // VOICE_GEN_BFYPRO
    SND_BANK_SPC_GA_BFYRI,   // VOICE_GEN_BFYRI
    SND_BANK_SPC_GA_BFYST,   // VOICE_GEN_BFYST
    SND_BANK_SPC_GA_BIKDRUG, // VOICE_GEN_BIKDRUG
    SND_BANK_SPC_GA_BIKERA,  // VOICE_GEN_BIKERA
    SND_BANK_SPC_GA_BIKERB,  // VOICE_GEN_BIKERB
    SND_BANK_SPC_GA_BMOCD,   // VOICE_GEN_BMOCD
    SND_BANK_SPC_GA_BMORI,   // VOICE_GEN_BMORI
    SND_BANK_SPC_GA_BMOSEC,  // VOICE_GEN_BMOSEC
    SND_BANK_SPC_GA_BMOST,   // VOICE_GEN_BMOST
    SND_BANK_SPC_GA_BMOTR1,  // VOICE_GEN_BMOTR1
    SND_BANK_SPC_GA_BMYAP,   // VOICE_GEN_BMYAP
    SND_BANK_SPC_GA_BMYBE,   // VOICE_GEN_BMYBE
    SND_BANK_SPC_GA_BMYBOUN, // VOICE_GEN_BMYBOUN
    SND_BANK_SPC_GA_BMYBOX,  // VOICE_GEN_BMYBOX
    SND_BANK_SPC_GA_BMYBU,   // VOICE_GEN_BMYBU
    SND_BANK_SPC_GA_BMYCG,   // VOICE_GEN_BMYCG
    SND_BANK_SPC_GA_BMYCON,  // VOICE_GEN_BMYCON
    SND_BANK_SPC_GA_BMYCR,   // VOICE_GEN_BMYCR
    SND_BANK_SPC_GA_BMYDJ,   // VOICE_GEN_BMYDJ
    SND_BANK_SPC_GA_BMYDRUG, // VOICE_GEN_BMYDRUG
    SND_BANK_SPC_GA_BMYMOUN, // VOICE_GEN_BMYMOUN
    SND_BANK_SPC_GA_BMYPOL1, // VOICE_GEN_BMYPOL1
    SND_BANK_SPC_GA_BMYPOL2, // VOICE_GEN_BMYPOL2
    SND_BANK_SPC_GA_BMYRI,   // VOICE_GEN_BMYRI
    SND_BANK_SPC_GA_BMYST,   // VOICE_GEN_BMYST
    SND_BANK_SPC_GA_BYMPI,   // VOICE_GEN_BYMPI
    SND_BANK_SPC_GA_CWFOFR,  // VOICE_GEN_CWFOFR
    SND_BANK_SPC_GA_CWFOHB,  // VOICE_GEN_CWFOHB
    SND_BANK_SPC_GA_CWFYFR1, // VOICE_GEN_CWFYFR1
    SND_BANK_SPC_GA_CWFYFR2, // VOICE_GEN_CWFYFR2
    SND_BANK_SPC_GA_CWFYHB1, // VOICE_GEN_CWFYHB1
    SND_BANK_SPC_GA_CWMOFR1, // VOICE_GEN_CWMOFR1
    SND_BANK_SPC_GA_CWMOHB1, // VOICE_GEN_CWMOHB1
    SND_BANK_SPC_GA_CWMOHB2, // VOICE_GEN_CWMOHB2
    SND_BANK_SPC_GA_CWMYFR,  // VOICE_GEN_CWMYFR
    SND_BANK_SPC_GA_CWMYHB1, // VOICE_GEN_CWMYHB1
    SND_BANK_SPC_GA_CWMYHB2, // VOICE_GEN_CWMYHB2
    SND_BANK_SPC_GA_DNFOLC1, // VOICE_GEN_DNFOLC1
    SND_BANK_SPC_GA_DNFOLC2, // VOICE_GEN_DNFOLC2
    SND_BANK_SPC_GA_DNFYLC,  // VOICE_GEN_DNFYLC
    SND_BANK_SPC_GA_DNMOLC1, // VOICE_GEN_DNMOLC1
    SND_BANK_SPC_GA_DNMOLC2, // VOICE_GEN_DNMOLC2
    SND_BANK_SPC_GA_DNMYLC,  // VOICE_GEN_DNMYLC
    SND_BANK_SPC_GA_DWFOLC,  // VOICE_GEN_DWFOLC
    SND_BANK_SPC_GA_DWFYLC1, // VOICE_GEN_DWFYLC1
    SND_BANK_SPC_GA_DWFYLC2, // VOICE_GEN_DWFYLC2
    SND_BANK_SPC_GA_DWMOLC1, // VOICE_GEN_DWMOLC1
    SND_BANK_SPC_GA_DWMOLC2, // VOICE_GEN_DWMOLC2
    SND_BANK_SPC_GA_DWMYLC1, // VOICE_GEN_DWMYLC1
    SND_BANK_SPC_GA_DWMYLC2, // VOICE_GEN_DWMYLC2
    SND_BANK_SPC_GA_HFORI,   // VOICE_GEN_HFORI
    SND_BANK_SPC_GA_HFOST,   // VOICE_GEN_HFOST
    SND_BANK_SPC_GA_HFYBE,   // VOICE_GEN_HFYBE
    SND_BANK_SPC_GA_HFYPRO,  // VOICE_GEN_HFYPRO
    SND_BANK_SPC_GA_HFYRI,   // VOICE_GEN_HFYRI
    SND_BANK_SPC_GA_HFYST,   // VOICE_GEN_HFYST
    SND_BANK_SPC_GA_HMORI,   // VOICE_GEN_HMORI
    SND_BANK_SPC_GA_HMOST,   // VOICE_GEN_HMOST
    SND_BANK_SPC_GA_HMYBE,   // VOICE_GEN_HMYBE
    SND_BANK_SPC_GA_HMYCM,   // VOICE_GEN_HMYCM
    SND_BANK_SPC_GA_HMYCR,   // VOICE_GEN_HMYCR
    SND_BANK_SPC_GA_HMYDRUG, // VOICE_GEN_HMYDRUG
    SND_BANK_SPC_GA_HMYRI,   // VOICE_GEN_HMYRI
    SND_BANK_SPC_GA_HMYST,   // VOICE_GEN_HMYST
    SND_BANK_SPC_GA_IMYST,   // VOICE_GEN_IMYST
    SND_BANK_SPC_GA_IRFYST,  // VOICE_GEN_IRFYST
    SND_BANK_SPC_GA_IRMYST,  // VOICE_GEN_IRMYST
    SND_BANK_SPC_GA_MAFFA,   // VOICE_GEN_MAFFA
    SND_BANK_SPC_GA_MAFFB,   // VOICE_GEN_MAFFB
    SND_BANK_SPC_GA_MALE01,  // VOICE_GEN_MALE01
    SND_BANK_SPC_GA_NOVOICE, // VOICE_GEN_NOVOICE
    SND_BANK_SPC_GA_OFORI,   // VOICE_GEN_OFORI
    SND_BANK_SPC_GA_OFOST,   // VOICE_GEN_OFOST
    SND_BANK_SPC_GA_OFYRI,   // VOICE_GEN_OFYRI
    SND_BANK_SPC_GA_OFYST,   // VOICE_GEN_OFYST
    SND_BANK_SPC_GA_OMOBOAT, // VOICE_GEN_OMOBOAT
    SND_BANK_SPC_GA_OMOKUNG, // VOICE_GEN_OMOKUNG
    SND_BANK_SPC_GA_OMORI,   // VOICE_GEN_OMORI
    SND_BANK_SPC_GA_OMOST,   // VOICE_GEN_OMOST
    SND_BANK_SPC_GA_OMYRI,   // VOICE_GEN_OMYRI
    SND_BANK_SPC_GA_OMYST,   // VOICE_GEN_OMYST
    SND_BANK_SPC_GA_SBFORI,  // VOICE_GEN_SBFORI
    SND_BANK_SPC_GA_SBFOST,  // VOICE_GEN_SBFOST
    SND_BANK_SPC_GA_SBFYPRO, // VOICE_GEN_SBFYPRO
    SND_BANK_SPC_GA_SBFYRI,  // VOICE_GEN_SBFYRI
    SND_BANK_SPC_GA_SBFYST,  // VOICE_GEN_SBFYST
    SND_BANK_SPC_GA_SBFYSTR, // VOICE_GEN_SBFYSTR
    SND_BANK_SPC_GA_SBMOCD,  // VOICE_GEN_SBMOCD
    SND_BANK_SPC_GA_SBMORI,  // VOICE_GEN_SBMORI
    SND_BANK_SPC_GA_SBMOST,  // VOICE_GEN_SBMOST
    SND_BANK_SPC_GA_SBMOTR1, // VOICE_GEN_SBMOTR1
    SND_BANK_SPC_GA_SBMOTR2, // VOICE_GEN_SBMOTR2
    SND_BANK_SPC_GA_SBMYCR,  // VOICE_GEN_SBMYCR
    SND_BANK_SPC_GA_SBMYRI,  // VOICE_GEN_SBMYRI
    SND_BANK_SPC_GA_SBMYST,  // VOICE_GEN_SBMYST
    SND_BANK_SPC_GA_SBMYTR3, // VOICE_GEN_SBMYTR3
    SND_BANK_SPC_GA_SFYPRO,  // VOICE_GEN_SFYPRO
    SND_BANK_SPC_GA_SHFYPRO, // VOICE_GEN_SHFYPRO
    SND_BANK_SPC_GA_SHMYCR,  // VOICE_GEN_SHMYCR
    SND_BANK_SPC_GA_SMYST,   // VOICE_GEN_SMYST
    SND_BANK_SPC_GA_SMYST2,  // VOICE_GEN_SMYST2
    SND_BANK_SPC_GA_SOFORI,  // VOICE_GEN_SOFORI
    SND_BANK_SPC_GA_SOFOST,  // VOICE_GEN_SOFOST
    SND_BANK_SPC_GA_SOFYBU,  // VOICE_GEN_SOFYBU
    SND_BANK_SPC_GA_SOFYRI,  // VOICE_GEN_SOFYRI
    SND_BANK_SPC_GA_SOFYST,  // VOICE_GEN_SOFYST
    SND_BANK_SPC_GA_SOMOBU,  // VOICE_GEN_SOMOBU
    SND_BANK_SPC_GA_SOMORI,  // VOICE_GEN_SOMORI
    SND_BANK_SPC_GA_SOMOST,  // VOICE_GEN_SOMOST
    SND_BANK_SPC_GA_SOMYAP,  // VOICE_GEN_SOMYAP
    SND_BANK_SPC_GA_SOMYBU,  // VOICE_GEN_SOMYBU
    SND_BANK_SPC_GA_SOMYRI,  // VOICE_GEN_SOMYRI
    SND_BANK_SPC_GA_SOMYST,  // VOICE_GEN_SOMYST
    SND_BANK_SPC_GA_SWFOPRO, // VOICE_GEN_SWFOPRO
    SND_BANK_SPC_GA_SWFORI,  // VOICE_GEN_SWFORI
    SND_BANK_SPC_GA_SWFOST,  // VOICE_GEN_SWFOST
    SND_BANK_SPC_GA_SWFYRI,  // VOICE_GEN_SWFYRI
    SND_BANK_SPC_GA_SWFYST,  // VOICE_GEN_SWFYST
    SND_BANK_SPC_GA_SWFYSTR, // VOICE_GEN_SWFYSTR
    SND_BANK_SPC_GA_SWMOCD,  // VOICE_GEN_SWMOCD
    SND_BANK_SPC_GA_SWMORI,  // VOICE_GEN_SWMORI
    SND_BANK_SPC_GA_SWMOST,  // VOICE_GEN_SWMOST
    SND_BANK_SPC_GA_SWMOTR1, // VOICE_GEN_SWMOTR1
    SND_BANK_SPC_GA_SWMOTR2, // VOICE_GEN_SWMOTR2
    SND_BANK_SPC_GA_SWMOTR3, // VOICE_GEN_SWMOTR3
    SND_BANK_SPC_GA_SWMOTR4, // VOICE_GEN_SWMOTR4
    SND_BANK_SPC_GA_SWMOTR5, // VOICE_GEN_SWMOTR5
    SND_BANK_SPC_GA_SWMYCR,  // VOICE_GEN_SWMYCR
    SND_BANK_SPC_GA_SWMYHP1, // VOICE_GEN_SWMYHP1
    SND_BANK_SPC_GA_SWMYHP2, // VOICE_GEN_SWMYHP2
    SND_BANK_SPC_GA_SWMYRI,  // VOICE_GEN_SWMYRI
    SND_BANK_SPC_GA_SWMYST,  // VOICE_GEN_SWMYST
    SND_BANK_SPC_GA_VBFYPRO, // VOICE_GEN_VBFYPRO
    SND_BANK_SPC_GA_VBFYST2, // VOICE_GEN_VBFYST2
    SND_BANK_SPC_GA_VBMOCD,  // VOICE_GEN_VBMOCD
    SND_BANK_SPC_GA_VBMYCR,  // VOICE_GEN_VBMYCR
    SND_BANK_SPC_GA_VBMYELV, // VOICE_GEN_VBMYELV
    SND_BANK_SPC_GA_VHFYPRO, // VOICE_GEN_VHFYPRO
    SND_BANK_SPC_GA_VHFYST3, // VOICE_GEN_VHFYST3
    SND_BANK_SPC_GA_VHMYCR,  // VOICE_GEN_VHMYCR
    SND_BANK_SPC_GA_VHMYELV, // VOICE_GEN_VHMYELV
    SND_BANK_SPC_GA_VIMYELV, // VOICE_GEN_VIMYELV
    SND_BANK_SPC_GA_VWFYPRO, // VOICE_GEN_VWFYPRO
    SND_BANK_SPC_GA_VWFYST1, // VOICE_GEN_VWFYST1
    SND_BANK_SPC_GA_VWFYWAI, // VOICE_GEN_VWFYWAI
    SND_BANK_SPC_GA_VWMOTR1, // VOICE_GEN_VWMOTR1
    SND_BANK_SPC_GA_VWMOTR2, // VOICE_GEN_VWMOTR2
    SND_BANK_SPC_GA_VWMYAP,  // VOICE_GEN_VWMYAP
    SND_BANK_SPC_GA_VWMYBJD, // VOICE_GEN_VWMYBJD
    SND_BANK_SPC_GA_VWMYCD,  // VOICE_GEN_VWMYCD
    SND_BANK_SPC_GA_VWMYCR,  // VOICE_GEN_VWMYCR
    SND_BANK_SPC_GA_WFOPJ,   // VOICE_GEN_WFOPJ
    SND_BANK_SPC_GA_WFORI,   // VOICE_GEN_WFORI
    SND_BANK_SPC_GA_WFOST,   // VOICE_GEN_WFOST
    SND_BANK_SPC_GA_WFYBE,   // VOICE_GEN_WFYBE
    SND_BANK_SPC_GA_WFYBU,   // VOICE_GEN_WFYBU
    SND_BANK_SPC_GA_WFYCRK,  // VOICE_GEN_WFYCRK
    SND_BANK_SPC_GA_WFYCRP,  // VOICE_GEN_WFYCRP
    SND_BANK_SPC_GA_WFYJG,   // VOICE_GEN_WFYJG
    SND_BANK_SPC_GA_WFYLG,   // VOICE_GEN_WFYLG
    SND_BANK_SPC_GA_WFYPRO,  // VOICE_GEN_WFYPRO
    SND_BANK_SPC_GA_WFYRI,   // VOICE_GEN_WFYRI
    SND_BANK_SPC_GA_WFYRO,   // VOICE_GEN_WFYRO
    SND_BANK_SPC_GA_WFYST,   // VOICE_GEN_WFYST
    SND_BANK_SPC_GA_WFYSTEW, // VOICE_GEN_WFYSTEW
    SND_BANK_SPC_GA_WMOMIB,  // VOICE_GEN_WMOMIB
    SND_BANK_SPC_GA_WMOPJ,   // VOICE_GEN_WMOPJ
    SND_BANK_SPC_GA_WMOPREA, // VOICE_GEN_WMOPREA
    SND_BANK_SPC_GA_WMORI,   // VOICE_GEN_WMORI
    SND_BANK_SPC_GA_WMOSCI,  // VOICE_GEN_WMOSCI
    SND_BANK_SPC_GA_WMOST,   // VOICE_GEN_WMOST
    SND_BANK_SPC_GA_WMOTR1,  // VOICE_GEN_WMOTR1
    SND_BANK_SPC_GA_WMYBE,   // VOICE_GEN_WMYBE
    SND_BANK_SPC_GA_WMYBMX,  // VOICE_GEN_WMYBMX
    SND_BANK_SPC_GA_WMYBOUN, // VOICE_GEN_WMYBOUN
    SND_BANK_SPC_GA_WMYBOX,  // VOICE_GEN_WMYBOX
    SND_BANK_SPC_GA_WMYBP,   // VOICE_GEN_WMYBP
    SND_BANK_SPC_GA_WMYBU,   // VOICE_GEN_WMYBU
    SND_BANK_SPC_GA_WMYCD1,  // VOICE_GEN_WMYCD1
    SND_BANK_SPC_GA_WMYCD2,  // VOICE_GEN_WMYCD2
    SND_BANK_SPC_GA_WMYCH,   // VOICE_GEN_WMYCH
    SND_BANK_SPC_GA_WMYCON,  // VOICE_GEN_WMYCON
    SND_BANK_SPC_GA_WMYCONB, // VOICE_GEN_WMYCONB
    SND_BANK_SPC_GA_WMYCR,   // VOICE_GEN_WMYCR
    SND_BANK_SPC_GA_WMYDRUG, // VOICE_GEN_WMYDRUG
    SND_BANK_SPC_GA_WMYGAR,  // VOICE_GEN_WMYGAR
    SND_BANK_SPC_GA_WMYGOL1, // VOICE_GEN_WMYGOL1
    SND_BANK_SPC_GA_WMYGOL2, // VOICE_GEN_WMYGOL2
    SND_BANK_SPC_GA_WMYJG,   // VOICE_GEN_WMYJG
    SND_BANK_SPC_GA_WMYLG,   // VOICE_GEN_WMYLG
    SND_BANK_SPC_GA_WMYMECH, // VOICE_GEN_WMYMECH
    SND_BANK_SPC_GA_WMYMOUN, // VOICE_GEN_WMYMOUN
    SND_BANK_SPC_GA_WMYPLT,  // VOICE_GEN_WMYPLT
    SND_BANK_SPC_GA_WMYRI,   // VOICE_GEN_WMYRI
    SND_BANK_SPC_GA_WMYRO,   // VOICE_GEN_WMYRO
    SND_BANK_SPC_GA_WMYSGRD, // VOICE_GEN_WMYSGRD
    SND_BANK_SPC_GA_WMYSKAT, // VOICE_GEN_WMYSKAT
    SND_BANK_SPC_GA_WMYST,   // VOICE_GEN_WMYST
    SND_BANK_SPC_GA_WMYTX1,  // VOICE_GEN_WMYTX1
    SND_BANK_SPC_GA_WMYTX2,  // VOICE_GEN_WMYTX2
    SND_BANK_SPC_GA_WMYVA,   // VOICE_GEN_WMYVA
};

// 0x8BA470 (Originally int16)
static constexpr std::array<eSoundBank, VOICE_EMG_END> gEmgSpeechVoiceToBankLookup = {
    SND_BANK_SPC_EA_ARMY1,   // VOICE_EMG_ARMY1
    SND_BANK_SPC_EA_ARMY2,   // VOICE_EMG_ARMY2
    SND_BANK_SPC_EA_ARMY3,   // VOICE_EMG_ARMY3
    SND_BANK_SPC_EA_EMT1,    // VOICE_EMG_EMT1
    SND_BANK_SPC_EA_EMT2,    // VOICE_EMG_EMT2
    SND_BANK_SPC_EA_EMT3,    // VOICE_EMG_EMT3
    SND_BANK_SPC_EA_EMT4,    // VOICE_EMG_EMT4
    SND_BANK_SPC_EA_EMT5,    // VOICE_EMG_EMT5
    SND_BANK_SPC_EA_FBI2,    // VOICE_EMG_FBI2
    SND_BANK_SPC_EA_FBI3,    // VOICE_EMG_FBI3
    SND_BANK_SPC_EA_FBI4,    // VOICE_EMG_FBI4
    SND_BANK_SPC_EA_FBI5,    // VOICE_EMG_FBI5
    SND_BANK_SPC_EA_FBI6,    // VOICE_EMG_FBI6
    SND_BANK_SPC_EA_LAPD1,   // VOICE_EMG_LAPD1
    SND_BANK_SPC_EA_LAPD2,   // VOICE_EMG_LAPD2
    SND_BANK_SPC_EA_LAPD3,   // VOICE_EMG_LAPD3
    SND_BANK_SPC_EA_LAPD4,   // VOICE_EMG_LAPD4
    SND_BANK_SPC_EA_LAPD5,   // VOICE_EMG_LAPD5
    SND_BANK_SPC_EA_LAPD6,   // VOICE_EMG_LAPD6
    SND_BANK_SPC_EA_LAPD7,   // VOICE_EMG_LAPD7
    SND_BANK_SPC_EA_LAPD8,   // VOICE_EMG_LAPD8
    SND_BANK_SPC_EA_LVPD1,   // VOICE_EMG_LVPD1
    SND_BANK_SPC_EA_LVPD2,   // VOICE_EMG_LVPD2
    SND_BANK_SPC_EA_LVPD3,   // VOICE_EMG_LVPD3
    SND_BANK_SPC_EA_LVPD4,   // VOICE_EMG_LVPD4
    SND_BANK_SPC_EA_LVPD5,   // VOICE_EMG_LVPD5
    SND_BANK_SPC_EA_MCOP1,   // VOICE_EMG_MCOP1
    SND_BANK_SPC_EA_MCOP2,   // VOICE_EMG_MCOP2
    SND_BANK_SPC_EA_MCOP3,   // VOICE_EMG_MCOP3
    SND_BANK_SPC_EA_MCOP4,   // VOICE_EMG_MCOP4
    SND_BANK_SPC_EA_MCOP5,   // VOICE_EMG_MCOP5
    SND_BANK_SPC_EA_MCOP6,   // VOICE_EMG_MCOP6
    SND_BANK_SPC_EA_PULASKI, // VOICE_EMG_PULASKI
    SND_BANK_SPC_EA_RCOP1,   // VOICE_EMG_RCOP1
    SND_BANK_SPC_EA_RCOP2,   // VOICE_EMG_RCOP2
    SND_BANK_SPC_EA_RCOP3,   // VOICE_EMG_RCOP3
    SND_BANK_SPC_EA_RCOP4,   // VOICE_EMG_RCOP4
    SND_BANK_SPC_EA_SFPD1,   // VOICE_EMG_SFPD1
    SND_BANK_SPC_EA_SFPD2,   // VOICE_EMG_SFPD2
    SND_BANK_SPC_EA_SFPD3,   // VOICE_EMG_SFPD3
    SND_BANK_SPC_EA_SFPD4,   // VOICE_EMG_SFPD4
    SND_BANK_SPC_EA_SFPD5,   // VOICE_EMG_SFPD5
    SND_BANK_SPC_EA_SWAT1,   // VOICE_EMG_SWAT1
    SND_BANK_SPC_EA_SWAT2,   // VOICE_EMG_SWAT2
    SND_BANK_SPC_EA_SWAT4,   // VOICE_EMG_SWAT4
    SND_BANK_SPC_EA_SWAT6,   // VOICE_EMG_SWAT6
};

// 0x8BBED0 (Originally int16)
static constexpr std::array<eSoundBank, VOICE_PLY_END> gPlySpeechVoiceToBankLookup = {
    SND_BANK_SPC_PA_AG,  // VOICE_PLY_AG
    SND_BANK_SPC_PA_AG2, // VOICE_PLY_AG2
    SND_BANK_SPC_PA_AR,  // VOICE_PLY_AR
    SND_BANK_SPC_PA_AR2, // VOICE_PLY_AR2
    SND_BANK_SPC_PA_CD,  // VOICE_PLY_CD
    SND_BANK_SPC_PA_CD2, // VOICE_PLY_CD2
    SND_BANK_SPC_PA_CF,  // VOICE_PLY_CF
    SND_BANK_SPC_PA_CF2, // VOICE_PLY_CF2
    SND_BANK_SPC_PA_CG,  // VOICE_PLY_CG
    SND_BANK_SPC_PA_CG2, // VOICE_PLY_CG2
    SND_BANK_SPC_PA_CR,  // VOICE_PLY_CR
    SND_BANK_SPC_PA_CR2, // VOICE_PLY_CR2
    SND_BANK_SPC_PA_PG,  // VOICE_PLY_PG
    SND_BANK_SPC_PA_PG2, // VOICE_PLY_PG2
    SND_BANK_SPC_PA_PR,  // VOICE_PLY_PR
    SND_BANK_SPC_PA_PR2, // VOICE_PLY_PR2
    SND_BANK_SPC_PA_WG,  // VOICE_PLY_WG
    SND_BANK_SPC_PA_WG2, // VOICE_PLY_WG2
    SND_BANK_SPC_PA_WR,  // VOICE_PLY_WR
    SND_BANK_SPC_PA_WR2, // VOICE_PLY_WR2
};

// 0x8BE5B8 (Originally int16)
static constexpr std::array<eSoundBank, VOICE_GNG_END> gGngSpeechVoiceToBankLookup = {
    SND_BANK_SPC_NA_BALLAS1,  // VOICE_GNG_BALLAS1
    SND_BANK_SPC_NA_BALLAS2,  // VOICE_GNG_BALLAS2
    SND_BANK_SPC_NA_BALLAS3,  // VOICE_GNG_BALLAS3
    SND_BANK_SPC_NA_BALLAS4,  // VOICE_GNG_BALLAS4
    SND_BANK_SPC_NA_BALLAS5,  // VOICE_GNG_BALLAS5
    SND_BANK_SPC_NA_BIG_BEAR, // VOICE_GNG_BIG_BEAR
    SND_BANK_SPC_NA_CESAR,    // VOICE_GNG_CESAR
    SND_BANK_SPC_NA_DNB1,     // VOICE_GNG_DNB1
    SND_BANK_SPC_NA_DNB2,     // VOICE_GNG_DNB2
    SND_BANK_SPC_NA_DNB3,     // VOICE_GNG_DNB3
    SND_BANK_SPC_NA_DNB5,     // VOICE_GNG_DNB5
    SND_BANK_SPC_NA_DWAINE,   // VOICE_GNG_DWAINE
    SND_BANK_SPC_NA_FAM1,     // VOICE_GNG_FAM1
    SND_BANK_SPC_NA_FAM2,     // VOICE_GNG_FAM2
    SND_BANK_SPC_NA_FAM3,     // VOICE_GNG_FAM3
    SND_BANK_SPC_NA_FAM4,     // VOICE_GNG_FAM4
    SND_BANK_SPC_NA_FAM5,     // VOICE_GNG_FAM5
    SND_BANK_SPC_NA_JIZZY,    // VOICE_GNG_JIZZY
    SND_BANK_SPC_NA_LSV1,     // VOICE_GNG_LSV1
    SND_BANK_SPC_NA_LSV2,     // VOICE_GNG_LSV2
    SND_BANK_SPC_NA_LSV3,     // VOICE_GNG_LSV3
    SND_BANK_SPC_NA_LSV4,     // VOICE_GNG_LSV4
    SND_BANK_SPC_NA_LSV5,     // VOICE_GNG_LSV5
    SND_BANK_SPC_NA_MACCER,   // VOICE_GNG_MACCER
    SND_BANK_SPC_NA_MAFBOSS,  // VOICE_GNG_MAFBOSS
    SND_BANK_SPC_NA_OGLOC,    // VOICE_GNG_OGLOC
    SND_BANK_SPC_NA_RYDER,    // VOICE_GNG_RYDER
    SND_BANK_SPC_NA_SFR1,     // VOICE_GNG_SFR1
    SND_BANK_SPC_NA_SFR2,     // VOICE_GNG_SFR2
    SND_BANK_SPC_NA_SFR3,     // VOICE_GNG_SFR3
    SND_BANK_SPC_NA_SFR4,     // VOICE_GNG_SFR4
    SND_BANK_SPC_NA_SFR5,     // VOICE_GNG_SFR5
    SND_BANK_SPC_NA_SMOKE,    // VOICE_GNG_SMOKE
    SND_BANK_SPC_NA_STRI1,    // VOICE_GNG_STRI1
    SND_BANK_SPC_NA_STRI2,    // VOICE_GNG_STRI2
    SND_BANK_SPC_NA_STRI4,    // VOICE_GNG_STRI4
    SND_BANK_SPC_NA_STRI5,    // VOICE_GNG_STRI5
    SND_BANK_SPC_NA_SWEET,    // VOICE_GNG_SWEET
    SND_BANK_SPC_NA_TBONE,    // VOICE_GNG_TBONE
    SND_BANK_SPC_NA_TORENO,   // VOICE_GNG_TORENO
    SND_BANK_SPC_NA_TRUTH,    // VOICE_GNG_TRUTH
    SND_BANK_SPC_NA_VLA1,     // VOICE_GNG_VLA1
    SND_BANK_SPC_NA_VLA2,     // VOICE_GNG_VLA2
    SND_BANK_SPC_NA_VLA3,     // VOICE_GNG_VLA3
    SND_BANK_SPC_NA_VLA4,     // VOICE_GNG_VLA4
    SND_BANK_SPC_NA_VLA5,     // VOICE_GNG_VLA5
    SND_BANK_SPC_NA_VMAFF1,   // VOICE_GNG_VMAFF1
    SND_BANK_SPC_NA_VMAFF2,   // VOICE_GNG_VMAFF2
    SND_BANK_SPC_NA_VMAFF3,   // VOICE_GNG_VMAFF3
    SND_BANK_SPC_NA_VMAFF4,   // VOICE_GNG_VMAFF4
    SND_BANK_SPC_NA_VMAFF5,   // VOICE_GNG_VMAFF5
    SND_BANK_SPC_NA_WOOZIE,   // VOICE_GNG_WOOZIE
};

// 0x8C4288 (Originally int16)
static constexpr std::array<eSoundBank, VOICE_GFD_END> gGfdSpeechVoiceToBankLookup = {
    SND_BANK_SPC_FA_BARBARA,  // VOICE_GFD_BARBARA
    SND_BANK_SPC_FA_BMOBAR,   // VOICE_GFD_BMOBAR
    SND_BANK_SPC_FA_BMYBARB,  // VOICE_GFD_BMYBARB
    SND_BANK_SPC_FA_BMYTATT,  // VOICE_GFD_BMYTATT
    SND_BANK_SPC_FA_CATALINA, // VOICE_GFD_CATALINA
    SND_BANK_SPC_FA_DENISE,   // VOICE_GFD_DENISE
    SND_BANK_SPC_FA_HELENA,   // VOICE_GFD_HELENA
    SND_BANK_SPC_FA_KATIE,    // VOICE_GFD_KATIE
    SND_BANK_SPC_FA_MICHELLE, // VOICE_GFD_MICHELLE
    SND_BANK_SPC_FA_MILLIE,   // VOICE_GFD_MILLIE
    SND_BANK_SPC_FA_POL_ANN,  // VOICE_GFD_POL_ANN
    SND_BANK_SPC_FA_WFYBURG,  // VOICE_GFD_WFYBURG
    SND_BANK_SPC_FA_WFYCLOT,  // VOICE_GFD_WFYCLOT
    SND_BANK_SPC_FA_WMYAMMO,  // VOICE_GFD_WMYAMMO
    SND_BANK_SPC_FA_WMYBARB,  // VOICE_GFD_WMYBARB
    SND_BANK_SPC_FA_WMYBELL,  // VOICE_GFD_WMYBELL
    SND_BANK_SPC_FA_WMYCLOT,  // VOICE_GFD_WMYCLOT
    SND_BANK_SPC_FA_WMYPIZZ,  // VOICE_GFD_WMYPIZZ
};

// 0x8C64AC
static constexpr std::array<eSoundBank, VOICE_PAIN_END> gPainVoiceToBankLookup = {
    SND_BANK_PAIN_A_CARL,   // VOICE_PAIN_FEMALE
    SND_BANK_PAIN_A_FEMALE, // VOICE_PAIN_MALE
    SND_BANK_PAIN_A_MALE,   // VOICE_PAIN_END
};
};

// clang-format on

void CAEPedSpeechAudioEntity::InjectHooks() {
    RH_ScopedVirtualClass(CAEPedSpeechAudioEntity, 0x85F310, 8);
    RH_ScopedCategory("Audio/Entities");

    RH_ScopedInstall(Constructor, 0x4E4F10);
    RH_ScopedInstall(IsGlobalContextImportantForInterupting, 0x4E4600);
    RH_ScopedInstall(IsGlobalContextUberImportant, 0x4E46F0);
    RH_ScopedInstall(GetNextMoodToUse, 0x4E4700);
    RH_ScopedInstall(GetVoiceForMood, 0x4E4760);
    RH_ScopedInstall(CanWePlayScriptedSpeech, 0x4E4950);
    RH_ScopedInstall(GetSpeechContextVolumeOffset, 0x4E4AE0);
    RH_ScopedInstall(RequestPedConversation, 0x4E50E0);
    RH_ScopedInstall(ReleasePedConversation, 0x4E52A0);
    RH_ScopedInstall(GetCurrentCJMood, 0x4E53B0);
    RH_ScopedInstall(StaticInitialise, 0x5B98C0);
    RH_ScopedInstall(GetSpecificSpeechContext, 0x4E4470);
    RH_ScopedInstall(Service, 0x4E3710);
    RH_ScopedInstall(Reset, 0x4E37B0);
    RH_ScopedInstall(ReservePedConversationSpeechSlots, 0x4E37F0);
    RH_ScopedInstall(ReservePlayerConversationSpeechSlot, 0x4E3870);
    RH_ScopedInstall(RequestPlayerConversation, 0x4E38C0);
    RH_ScopedInstall(ReleasePlayerConversation, 0x4E3960);
    RH_ScopedInstall(SetUpConversation, 0x4E3A00);
    RH_ScopedInstall(GetAudioPedType, 0x4E3C60);
    RH_ScopedInstall(GetVoice, 0x4E3CD0);
    RH_ScopedInstall(DisableAllPedSpeech, 0x4E3EB0);
    RH_ScopedInstall(IsGlobalContextPain, 0x4E44F0);
    RH_ScopedInstall(SetCJMood, 0x4E3ED0);
    RH_ScopedInstall(EnableAllPedSpeech, 0x4E3EC0);
    RH_ScopedInstall(IsCJDressedInForGangSpeech, 0x4E4270);
    RH_ScopedInstall(GetSexForSpecialPed, 0x4E4260);
    RH_ScopedInstall(IsGlobalContextImportantForWidescreen, 0x4E46B0);
    RH_ScopedInstall(GetRepeatTime, 0x4E47E0);
    RH_ScopedInstall(LoadAndPlaySpeech, 0x4E4840);
    RH_ScopedInstall(GetNumSlotsPlayingContext, 0x4E49B0);
    RH_ScopedInstall(GetNextPlayTime, 0x4E49E0);
    RH_ScopedInstall(SetNextPlayTime, 0x4E4A20);
    RH_ScopedInstall(DisablePedSpeech, 0x4E56D0);
    RH_ScopedInstall(DisablePedSpeechForScriptSpeech, 0x4E5700);
    RH_ScopedInstall(CanPedSayGlobalContext, 0x4E5730);
    RH_ScopedInstall(GetVoiceAndTypeFromModel, 0x4E58C0);
    RH_ScopedInstall(GetSoundAndBankIDs, 0x4E5920);
    RH_ScopedInstall(CanWePlayGlobalSpeechContext, 0x4E5F10);
    RH_ScopedInstall(AddSayEvent, 0x4E6550);
    RH_ScopedInstall(Initialise, 0x4E68D0);
    RH_ScopedInstall(CanPedHoldConversation, 0x4E69E0);
    RH_ScopedInstall(IsGlobalContextImportantForStreaming, 0x4E4510);
    RH_ScopedInstall(EnablePedSpeech, 0x4E3F70);
    RH_ScopedInstall(EnablePedSpeechForScriptSpeech, 0x4E3F90);
    RH_ScopedInstall(StopCurrentSpeech, 0x4E3FB0, { .reversed = false });
    RH_ScopedInstall(GetSoundAndBankIDsForScriptedSpeech, 0x4E4400, { .reversed = false });
    RH_ScopedInstall(GetSexFromModel, 0x4E4200, { .reversed = false });
    RH_ScopedInstall(GetPedTalking, 0x4E3F50);
    RH_ScopedInstall(GetVoiceAndTypeForSpecialPed, 0x4E4170, { .reversed = false });
    RH_ScopedVMTInstall(UpdateParameters, 0x4E3520, { .reversed = false });
    RH_ScopedVMTInstall(AddScriptSayEvent, 0x4E4F70, { .reversed = false });
    RH_ScopedVMTInstall(Terminate, 0x4E5670, { .reversed = false });
    RH_ScopedVMTInstall(PlayLoadedSound, 0x4E5CD0, { .reversed = false });
    RH_ScopedVMTInstall(GetAllocatedVoice, 0x4E4120);
    RH_ScopedVMTInstall(WillPedChatAboutTopic, 0x4E5800);
    RH_ScopedVMTInstall(GetPedType, 0x4E4130);
    RH_ScopedVMTInstall(IsPedFemaleForAudio, 0x4E4150);
}

CAEPedSpeechAudioEntity::CAEPedSpeechAudioEntity() noexcept {
    m_NextTimeCanSayPain.fill(0);
}

CAEPedSpeechAudioEntity::CAEPedSpeechAudioEntity(CPed* ped) noexcept :
    CAEPedSpeechAudioEntity{}
{
    m_pEntity = ped;
    if (ped->GetModelID() != MODEL_INVALID) {
        auto* const mi = ped->GetPedModelInfo();
        if (mi->m_nPedAudioType == PED_TYPE_SPC) {
            if (!GetVoiceAndTypeForSpecialPed(mi->m_nKey)) {
                return;
            }
        } else {
            m_VoiceID = mi->m_nVoiceId;
            if (m_VoiceID == -1) {
                return;
            }
            mi->IncrementVoice();
        }
        VERIFY(GetSexFromModel(ped->GetModelID()));
        m_IsInitialized = true;
    }
}

// 0x4E4700
int16 __stdcall CAEPedSpeechAudioEntity::GetNextMoodToUse(eCJMood lastMood) {
    switch (lastMood) {
    case MOOD_AG:
    case MOOD_PR: return MOOD_AR;
    case MOOD_PG: return MOOD_AG;
    case MOOD_WG: return MOOD_CG;
    default:      return MOOD_CR;
    }
}

// 0x4E4760
int32 __stdcall CAEPedSpeechAudioEntity::GetVoiceForMood(int16 mood) {
    auto rnd = CAEAudioUtility::GetRandomNumberInRange(0, 1);
    if (mood < 0 || mood >= 10)
        return rnd + 10;
    return rnd + 2 * mood;
}

// 0x4E4950
int16 CAEPedSpeechAudioEntity::CanWePlayScriptedSpeech() {
    for (auto i = 0; i < PED_TYPE_SPC; i++) {
        const auto slot = (s_NextSpeechSlot + i) % PED_TYPE_SPC;
        if (s_PedSpeechSlots[slot].Status == CAEPedSpeechSlot::eStatus::FREE) {
            s_NextSpeechSlot = slot;
            return slot;
        }
    }
    return -1;
}

// 0x4E4AE0
float CAEPedSpeechAudioEntity::GetSpeechContextVolumeOffset(eGlobalSpeechContextS16 gctx) {
    const auto CalculateStrainVolumeOffset = [](float base) {
        const auto t = std::clamp(CStats::GetStatValue(STAT_FAT) - CStats::GetStatValue(STAT_MUSCLE), -1.f, 1.f);
        return (t + 1.f) * 0.5f * base + base;
    };
    switch (gctx) {
    case CTX_GLOBAL_STOMACH_RUMBLE:        return -6.f; // 0x4E4CBB
    case CTX_GLOBAL_BREATHING:             return -12.f; // 0x4E4CC7
    case CTX_GLOBAL_PAIN_CJ_STRAIN:        return CalculateStrainVolumeOffset(-18.f); // 0x4E4B08
    case CTX_GLOBAL_PAIN_CJ_STRAIN_EXHALE: return CalculateStrainVolumeOffset(-21.f); // 0x4E4BE2
    case CTX_GLOBAL_PAIN_CJ_DROWNING:      return 3.f; // 0x4E4CBE
    default:                         return 0.f; // 0x4E4CD0
    }
}

// 0x4E50E0
bool CAEPedSpeechAudioEntity::RequestPedConversation(CPed* pedA, CPed* pedB) {
    if (s_bAllSpeechDisabled || s_bPedConversationHappening || s_bPlayerConversationHappening) {
        return false;
    }
    if (pedA->m_pedSpeech.GetAllocatedVoice() == pedB->m_pedSpeech.GetAllocatedVoice()) {
        return false;
    }
    const auto CheckCanPedTalk = [](CPed* p) {
        return !p->m_pedSpeech.IsAllSpeechDisabled()
            && p->CanPedHoldConversation()
            && !p->GetPedTalking()
            && CVector::DistSqr(TheCamera.GetPosition(), p->GetPosition()) <= sq(40.f);
    };
    if (!CheckCanPedTalk(pedA) || !CheckCanPedTalk(pedB)) {
        return false;
    }
    if (!ReservePedConversationSpeechSlots()) {
        return false;
    }
    s_bPedConversationHappening = true;
    s_pConversationPed1         = pedA;
    s_pConversationPed2         = pedB;
    SetUpConversation();
    return true;
}

// 0x4E52A0
void CAEPedSpeechAudioEntity::ReleasePedConversation() {
    if (!s_bPedConversationHappening) {
        return;
    }
    if (s_pConversationPedSlot1 == -1 || s_pConversationPedSlot2 == -1) {
        return;
    }

    const auto ReleaseConversationFromSlot = [](CPed*& convoPed, auto& slotID) {
        auto& ss = s_PedSpeechSlots[slotID];
        switch (ss.Status) {
        case CAEPedSpeechSlot::eStatus::FREE:
        case CAEPedSpeechSlot::eStatus::RESERVED:
            break;
        default:
            ss.AudioEntity->StopCurrentSpeech();
        }
        ss       = {}; // Reset slot
        slotID   = -1;
        convoPed = nullptr;
    };
    ReleaseConversationFromSlot(s_pConversationPed1, s_pConversationPedSlot1);
    ReleaseConversationFromSlot(s_pConversationPed2, s_pConversationPedSlot2);

    s_bPedConversationHappening = false;
}

// 0x4E53B0
int16 CAEPedSpeechAudioEntity::GetCurrentCJMood() {
    const auto* const plyr = FindPlayerPed();
    if (!plyr) {
        return MOOD_CR;
    }

    const auto isMoodOverrideActive = s_nCJMoodOverrideTime >= CTimer::GetTimeInMS();
    
    const auto CheckCJIsWellDressed = [&] { // 0x4E53DF
        if (isMoodOverrideActive && s_nCJWellDressed != -1) {
            return s_nCJWellDressed != 0;
        }
        return CStats::GetStatValue(STAT_CLOTHES_RESPECT) >= 650.f
            && CStats::GetStatValue(STAT_FASHION_BUDGET) >= 10'000.f;
    };

    const auto CheckCJIsGangBanging = [&] { // 0x4E5440
        if (isMoodOverrideActive && s_nCJGangBanging != -1) {
            return s_nCJGangBanging != 0;
        }
        if (IsCJDressedInForGangSpeech()) {
            return true;
        }
        auto& plyrGrp = plyr->GetPlayerGroup();
        if (plyrGrp.GetMembership().CountMembersExcludingLeader() <= 1) {
            return true;
        }
        const auto& mem = plyrGrp.GetMembership().GetMembers().front(); // The one-and-only member (This isnt the same as `GetMember(0)`!!!)
        if (mem.m_nPedType == PED_TYPE_GANG2) {
            return true;
        }
        auto& memSpeech = mem.m_pedSpeech;
        return memSpeech.m_PedAudioType == PED_TYPE_GANG
            && notsa::contains({ VOICE_GNG_RYDER, VOICE_GNG_SWEET, VOICE_GNG_SMOKE }, (eGngSpeechVoices)memSpeech.m_VoiceID);
    };

    const auto CheckCJIsFat = [&] { // 0x4E54E0
        if (isMoodOverrideActive && s_nCJFat != -1) {
            return s_nCJFat != 0;
        }
        return CStats::GetStatValue(STAT_FAT) >= 600.f
            && CStats::GetStatValue(STAT_FAT) - 200.f > CStats::GetStatValue(STAT_MUSCLE);
    };

    const auto DeriveMood = [&](eCJMood basicMood) { // 0x4E55CC
        if (isMoodOverrideActive && s_nCJBasicMood != MOOD_UNK) { // 0x4E5592
            basicMood = s_nCJBasicMood;
        }
        switch (basicMood) {
        case MOOD_AR:
            return CheckCJIsFat()
                ? MOOD_AR
                : MOOD_AG;
        case MOOD_CR: {
            if (CheckCJIsGangBanging()) {
                return MOOD_CG;
            }
            if (CheckCJIsFat()) {
                return MOOD_CF;
            }
            return CheckCJIsWellDressed()
                ? MOOD_CD
                : MOOD_CR;
        }
        case MOOD_PR:
            return CheckCJIsGangBanging()
                ? MOOD_PG
                : MOOD_PR;
        case MOOD_WR:
            return CheckCJIsGangBanging()
                ? MOOD_WG
                : MOOD_WR;
        default:
            return MOOD_CR;
        }
    };

    if (FindPlayerWanted()->GetWantedLevel() > 3) { // 0x4E5537
        return DeriveMood(MOOD_PR);
    }

    if (FindPlayerWanted()->GetWantedLevel() > 1) { // 0x4E554B
        return DeriveMood(MOOD_AR);
    }

    if (CTheScripts::LastMissionPassedTime == -1) { // 0x4E555C
        if (CTimer::GetTimeInMS() >= CTheScripts::LastMissionPassedTime) {
            return CTimer::GetTimeInMS() < CTheScripts::LastMissionPassedTime + 180'000
                ? DeriveMood(MOOD_WR)
                : DeriveMood(MOOD_CR);
        } else {
            CTheScripts::LastMissionPassedTime = CTimer::GetTimeInMS();
        }
    }

    return DeriveMood(MOOD_CR); // 0x4E5579
}

// 0x5B98C0
void CAEPedSpeechAudioEntity::StaticInitialise() {
    rng::fill(s_PedSpeechSlots, CAEPedSpeechSlot{});
    rng::fill(s_PhraseMemory, tPhraseMemory{});

    Reset();

    s_pConversationPed1            = nullptr;
    s_pConversationPed2            = nullptr;
    s_pConversationPedSlot1        = 0;
    s_pConversationPedSlot2        = 0;
    s_pPlayerConversationPed       = nullptr;
    s_bPedConversationHappening    = false;
    s_bPlayerConversationHappening = false;
    rng::fill(s_Conversation, CTX_GLOBAL_UNK);

    s_nCJBasicMood        = MOOD_UNK;
    s_nCJGangBanging      = -1;
    s_nCJFat              = -1;
    s_nCJWellDressed      = -1;
    s_nCJMoodOverrideTime = 0;

    s_NextSpeechSlot     = 0;
    s_bAllSpeechDisabled = false;
    s_bAPlayerSpeaking   = false;
    s_bForceAudible      = false;
}

// 0x4E4470
eSpecificSpeechContext CAEPedSpeechAudioEntity::GetSpecificSpeechContext(eGlobalSpeechContext gCtx, eAudioPedType pedAudioType) {
    // Omitted useless `if`s
    // NOTE: Original code allowed `PED_TYPE_SPC` as a valid ped type too... but that (technically) caused an out-of-bounds read...
    //       We won't assert on that, because there are bounds check for the array (as we use std::array)
    if (const auto* const ctxi = GetGlobalSpeechContextInfo(gCtx)) {
        return ctxi->SpecificSpeechContext[pedAudioType];
    }
    return -1;
}

// 0x4E3710
void CAEPedSpeechAudioEntity::Service() {
    s_bForceAudible = false;
    for (auto&& [i, ss] : notsa::enumerate(s_PedSpeechSlots)) {
        // Waiting for sound to load, and has loaded?
        if (ss.Status == CAEPedSpeechSlot::eStatus::LOADING && AEAudioHardware.IsSoundLoaded(ss.SoundBankID, ss.SoundID, SND_BANK_SLOT_SPEECH1 + i)) {
            ss.Status = CAEPedSpeechSlot::eStatus::WAITING;
        }

        // Sound is now loaded, waiting to be played
        if (ss.Status == CAEPedSpeechSlot::eStatus::WAITING) {
            if (ss.StartPlaybackTime >= CTimer::GetTimeInMS()) {
                if (const auto ae = ss.AudioEntity) {
                    ae->PlayLoadedSound();
                } else {
                    ss.Status = CAEPedSpeechSlot::eStatus::FREE;
                }
            }
        }

        // `PlayLoadedSound` above might've modified the status, must check it again (can't use switch for whole code)
        switch (ss.Status) {
        case CAEPedSpeechSlot::eStatus::REQUESTED:
        case CAEPedSpeechSlot::eStatus::PLAYING: {
            s_bForceAudible |= ss.ForceAudible;
        }
        }
    }
}

// 0x4E37B0
void CAEPedSpeechAudioEntity::Reset() {
    for (auto& time : gGlobalSpeechContextNextPlayTime) {
        time = CTimer::GetTimeInMS() + CAEAudioUtility::GetRandomNumberInRange(3000, 7000);
    }
    s_nCJMoodOverrideTime = 0;
}

// 0x4E37F0
bool CAEPedSpeechAudioEntity::ReservePedConversationSpeechSlots() {
    const auto slotA = GetFreeSpeechSlot();
    const auto slotB = GetFreeSpeechSlot();

    if (slotA == -1 || slotB == -1) {
        return false;
    }

    const auto SetupSlot = [](int32 slot) {
        auto* const ss                   = &s_PedSpeechSlots[slot];
        ss->Status                       = CAEPedSpeechSlot::eStatus::RESERVED;
        ss->IsReservedForPedConversation = true;
    };
    SetupSlot(s_pConversationPedSlot1 = slotA);
    SetupSlot(s_pConversationPedSlot2 = slotB);

    return true;
}

// 0x4E3870
bool CAEPedSpeechAudioEntity::ReservePlayerConversationSpeechSlot() {
    const auto slot = GetFreeSpeechSlot();
    if (slot == -1) {
        return false;
    }
    s_pConversationPedSlot1             = slot;
    auto* const ss                      = &s_PedSpeechSlots[slot];
    ss->Status                          = CAEPedSpeechSlot::eStatus::RESERVED;
    ss->IsReservedForPlayerConversation = true;

    return true;
}

// 0x4E38C0
bool CAEPedSpeechAudioEntity::RequestPlayerConversation(CPed* ped) {
    if (s_bAllSpeechDisabled) {
        return false;
    }

    if (ped->m_pedSpeech.m_IsSpeechForScriptsDisabled || ped->m_pedSpeech.m_IsSpeechDisabled) {
        return false;
    }

    const auto player = FindPlayerPed();
    if (!player || player->m_pedSpeech.m_IsSpeechDisabled || player->m_pedSpeech.m_IsSpeechForScriptsDisabled) {
        return false;
    }

    if (   s_bPedConversationHappening
        || s_bPlayerConversationHappening
        || ped->GetPedTalking()
        || !ReservePlayerConversationSpeechSlot()
    ) {
        return false;
    }

    s_pPlayerConversationPed       = ped;
    s_bPlayerConversationHappening = true;

    return true;
}

// 0x4E3960
void CAEPedSpeechAudioEntity::ReleasePlayerConversation() {
    if (!s_bPlayerConversationHappening) {
        return;
    }
    s_bPlayerConversationHappening = false;
    if (s_pConversationPedSlot1 < 0) {
        return;
    }
    auto* const ss = &s_PedSpeechSlots[s_pConversationPedSlot1];
    switch (ss->Status) {
    case CAEPedSpeechSlot::eStatus::FREE:
    case CAEPedSpeechSlot::eStatus::RESERVED: {
        *ss = {};
    }
    }
    s_PedSpeechSlots[s_pConversationPedSlot1] = {};
    s_pConversationPedSlot1                   = -1;
    s_pPlayerConversationPed                  = nullptr;
}

// 0x4E3A00
void CAEPedSpeechAudioEntity::SetUpConversation() {
    rng::fill(s_Conversation, CTX_GLOBAL_NO_SPEECH);

    auto PushConvo = [i = 0](eGlobalSpeechContext gctx) mutable {
        s_Conversation[i++] = gctx;
    };

    const auto PushConvoForPeds = [&](eGlobalSpeechContext ifFemale, eGlobalSpeechContext ifMale) {
        const auto PushConvoForPed = [&](const CPed* p) {
            switch (p->m_nPedType) {
            case PED_TYPE_PROSTITUTE:
            case PED_TYPE_CIVFEMALE: PushConvo(ifFemale); break;
            default:                 PushConvo(ifMale);   break;
            }
        };
        PushConvoForPed(s_pConversationPed2); // yes, ped 2, then ped 1
        PushConvoForPed(s_pConversationPed1);
    };

    // Greeting
    PushConvoForPeds(CTX_GLOBAL_PCONV_GREET_FEM, CTX_GLOBAL_PCONV_GREET_MALE);

    // Other stuff
    const auto r = CAEAudioUtility::GetRandomNumberInRange(1, 10);
    if (r <= 8) { // Combined cases 1, -> 4 + 5, -> 8
        const auto hasInitialState = r <= 4;

        s_ConversationLength = hasInitialState ? 3 : 2;

        if (hasInitialState) { // For cases 1 -> 4 only
            const auto isGood = CGeneral::DoCoinFlip();
            PushConvo(isGood ?  CTX_GLOBAL_PCONV_STATE_GOOD :  CTX_GLOBAL_PCONV_STATE_BAD);
            PushConvo(isGood ?  CTX_GLOBAL_PCONV_AGREE_GOOD :  CTX_GLOBAL_PCONV_AGREE_BAD);
        }

        // A question
        PushConvo(CTX_GLOBAL_PCONV_QUESTION);

        // Whenever they agree with the answer (?)
        switch (CAEAudioUtility::GetRandomNumberInRange(0, 2)) {
        case 0: PushConvo(CTX_GLOBAL_PCONV_AGREE_GOOD); break;
        case 1: PushConvo(CTX_GLOBAL_PCONV_AGREE_BAD);  break;
        case 2: PushConvo(CTX_GLOBAL_PCONV_ANS_NO);     break;
        }

        // Part away (?)
        PushConvoForPeds(CTX_GLOBAL_PCONV_PART_FEM, CTX_GLOBAL_PCONV_PART_MALE);
    } else if (r <= 10) { // cases 9, 10
        s_ConversationLength = 1;

        PushConvo(CGeneral::DoCoinFlip() ?  CTX_GLOBAL_PCONV_STATE_GOOD :  CTX_GLOBAL_PCONV_STATE_BAD);
        PushConvo(CTX_GLOBAL_PCONV_DISMISS);
    } else {
        NOTSA_UNREACHABLE();
    }
}

// 0x4E3C60
eAudioPedType CAEPedSpeechAudioEntity::GetAudioPedType(const char* name) {
    static const auto mapping = notsa::make_mapping<std::string_view, eAudioPedType>({
        {"PED_TYPE_GEN",    PED_TYPE_GEN   },
        {"PED_TYPE_EMG",    PED_TYPE_EMG   },
        {"PED_TYPE_PLAYER", PED_TYPE_PLAYER},
        {"PED_TYPE_GANG",   PED_TYPE_GANG  },
        {"PED_TYPE_GFD",    PED_TYPE_GFD   },
        {"PED_TYPE_SPC",    PED_TYPE_SPC   },
    });
    return notsa::find_value_or(mapping, name, PED_TYPE_UNK);
}

// 0x4E3CD0
ePedSpeechVoiceS16 CAEPedSpeechAudioEntity::GetVoice(const char* name, eAudioPedTypeS16 type) {
    switch (type) {
    case PED_TYPE_GEN: {
        static const auto mapping = notsa::make_mapping<std::string_view, eGenSpeechVoices>({
            {"VOICE_GEN_BBDYG1", VOICE_GEN_BBDYG1}, {"VOICE_GEN_BBDYG2", VOICE_GEN_BBDYG2}, {"VOICE_GEN_BFORI", VOICE_GEN_BFORI}, {"VOICE_GEN_BFOST", VOICE_GEN_BFOST}, {"VOICE_GEN_BFYBE", VOICE_GEN_BFYBE}, {"VOICE_GEN_BFYBU", VOICE_GEN_BFYBU}, {"VOICE_GEN_BFYCRP", VOICE_GEN_BFYCRP}, {"VOICE_GEN_BFYPRO", VOICE_GEN_BFYPRO}, {"VOICE_GEN_BFYRI", VOICE_GEN_BFYRI}, {"VOICE_GEN_BFYST", VOICE_GEN_BFYST}, {"VOICE_GEN_BIKDRUG", VOICE_GEN_BIKDRUG}, {"VOICE_GEN_BIKERA", VOICE_GEN_BIKERA}, {"VOICE_GEN_BIKERB", VOICE_GEN_BIKERB}, {"VOICE_GEN_BMOCD", VOICE_GEN_BMOCD}, {"VOICE_GEN_BMORI", VOICE_GEN_BMORI}, {"VOICE_GEN_BMOSEC", VOICE_GEN_BMOSEC}, {"VOICE_GEN_BMOST", VOICE_GEN_BMOST}, {"VOICE_GEN_BMOTR1", VOICE_GEN_BMOTR1}, {"VOICE_GEN_BMYAP", VOICE_GEN_BMYAP}, {"VOICE_GEN_BMYBE", VOICE_GEN_BMYBE}, {"VOICE_GEN_BMYBOUN", VOICE_GEN_BMYBOUN}, {"VOICE_GEN_BMYBOX", VOICE_GEN_BMYBOX}, {"VOICE_GEN_BMYBU", VOICE_GEN_BMYBU}, {"VOICE_GEN_BMYCG", VOICE_GEN_BMYCG}, {"VOICE_GEN_BMYCON", VOICE_GEN_BMYCON}, {"VOICE_GEN_BMYCR", VOICE_GEN_BMYCR}, {"VOICE_GEN_BMYDJ", VOICE_GEN_BMYDJ}, {"VOICE_GEN_BMYDRUG", VOICE_GEN_BMYDRUG}, {"VOICE_GEN_BMYMOUN", VOICE_GEN_BMYMOUN}, {"VOICE_GEN_BMYPOL1", VOICE_GEN_BMYPOL1}, {"VOICE_GEN_BMYPOL2", VOICE_GEN_BMYPOL2}, {"VOICE_GEN_BMYRI", VOICE_GEN_BMYRI}, {"VOICE_GEN_BMYST", VOICE_GEN_BMYST}, {"VOICE_GEN_BYMPI", VOICE_GEN_BYMPI}, {"VOICE_GEN_CWFOFR", VOICE_GEN_CWFOFR}, {"VOICE_GEN_CWFOHB", VOICE_GEN_CWFOHB}, {"VOICE_GEN_CWFYFR1", VOICE_GEN_CWFYFR1}, {"VOICE_GEN_CWFYFR2", VOICE_GEN_CWFYFR2}, {"VOICE_GEN_CWFYHB1", VOICE_GEN_CWFYHB1}, {"VOICE_GEN_CWMOFR1", VOICE_GEN_CWMOFR1}, {"VOICE_GEN_CWMOHB1", VOICE_GEN_CWMOHB1}, {"VOICE_GEN_CWMOHB2", VOICE_GEN_CWMOHB2}, {"VOICE_GEN_CWMYFR", VOICE_GEN_CWMYFR}, {"VOICE_GEN_CWMYHB1", VOICE_GEN_CWMYHB1}, {"VOICE_GEN_CWMYHB2", VOICE_GEN_CWMYHB2}, {"VOICE_GEN_DNFOLC1", VOICE_GEN_DNFOLC1}, {"VOICE_GEN_DNFOLC2", VOICE_GEN_DNFOLC2}, {"VOICE_GEN_DNFYLC", VOICE_GEN_DNFYLC}, {"VOICE_GEN_DNMOLC1", VOICE_GEN_DNMOLC1}, {"VOICE_GEN_DNMOLC2", VOICE_GEN_DNMOLC2}, {"VOICE_GEN_DNMYLC", VOICE_GEN_DNMYLC}, {"VOICE_GEN_DWFOLC", VOICE_GEN_DWFOLC}, {"VOICE_GEN_DWFYLC1", VOICE_GEN_DWFYLC1}, {"VOICE_GEN_DWFYLC2", VOICE_GEN_DWFYLC2}, {"VOICE_GEN_DWMOLC1", VOICE_GEN_DWMOLC1}, {"VOICE_GEN_DWMOLC2", VOICE_GEN_DWMOLC2}, {"VOICE_GEN_DWMYLC1", VOICE_GEN_DWMYLC1}, {"VOICE_GEN_DWMYLC2", VOICE_GEN_DWMYLC2}, {"VOICE_GEN_HFORI", VOICE_GEN_HFORI}, {"VOICE_GEN_HFOST", VOICE_GEN_HFOST}, {"VOICE_GEN_HFYBE", VOICE_GEN_HFYBE}, {"VOICE_GEN_HFYPRO", VOICE_GEN_HFYPRO}, {"VOICE_GEN_HFYRI", VOICE_GEN_HFYRI}, {"VOICE_GEN_HFYST", VOICE_GEN_HFYST}, {"VOICE_GEN_HMORI", VOICE_GEN_HMORI}, {"VOICE_GEN_HMOST", VOICE_GEN_HMOST}, {"VOICE_GEN_HMYBE", VOICE_GEN_HMYBE}, {"VOICE_GEN_HMYCM", VOICE_GEN_HMYCM}, {"VOICE_GEN_HMYCR", VOICE_GEN_HMYCR}, {"VOICE_GEN_HMYDRUG", VOICE_GEN_HMYDRUG}, {"VOICE_GEN_HMYRI", VOICE_GEN_HMYRI}, {"VOICE_GEN_HMYST", VOICE_GEN_HMYST}, {"VOICE_GEN_IMYST", VOICE_GEN_IMYST}, {"VOICE_GEN_IRFYST", VOICE_GEN_IRFYST}, {"VOICE_GEN_IRMYST", VOICE_GEN_IRMYST}, {"VOICE_GEN_MAFFA", VOICE_GEN_MAFFA}, {"VOICE_GEN_MAFFB", VOICE_GEN_MAFFB}, {"VOICE_GEN_MALE01", VOICE_GEN_MALE01}, {"VOICE_GEN_NOVOICE", VOICE_GEN_NOVOICE}, {"VOICE_GEN_OFORI", VOICE_GEN_OFORI}, {"VOICE_GEN_OFOST", VOICE_GEN_OFOST}, {"VOICE_GEN_OFYRI", VOICE_GEN_OFYRI}, {"VOICE_GEN_OFYST", VOICE_GEN_OFYST}, {"VOICE_GEN_OMOBOAT", VOICE_GEN_OMOBOAT}, {"VOICE_GEN_OMOKUNG", VOICE_GEN_OMOKUNG}, {"VOICE_GEN_OMORI", VOICE_GEN_OMORI}, {"VOICE_GEN_OMOST", VOICE_GEN_OMOST}, {"VOICE_GEN_OMYRI", VOICE_GEN_OMYRI}, {"VOICE_GEN_OMYST", VOICE_GEN_OMYST}, {"VOICE_GEN_SBFORI", VOICE_GEN_SBFORI}, {"VOICE_GEN_SBFOST", VOICE_GEN_SBFOST}, {"VOICE_GEN_SBFYPRO", VOICE_GEN_SBFYPRO}, {"VOICE_GEN_SBFYRI", VOICE_GEN_SBFYRI}, {"VOICE_GEN_SBFYST", VOICE_GEN_SBFYST}, {"VOICE_GEN_SBFYSTR", VOICE_GEN_SBFYSTR}, {"VOICE_GEN_SBMOCD", VOICE_GEN_SBMOCD}, {"VOICE_GEN_SBMORI", VOICE_GEN_SBMORI}, {"VOICE_GEN_SBMOST", VOICE_GEN_SBMOST}, {"VOICE_GEN_SBMOTR1", VOICE_GEN_SBMOTR1}, {"VOICE_GEN_SBMOTR2", VOICE_GEN_SBMOTR2}, {"VOICE_GEN_SBMYCR", VOICE_GEN_SBMYCR}, {"VOICE_GEN_SBMYRI", VOICE_GEN_SBMYRI}, {"VOICE_GEN_SBMYST", VOICE_GEN_SBMYST}, {"VOICE_GEN_SBMYTR3", VOICE_GEN_SBMYTR3}, {"VOICE_GEN_SFYPRO", VOICE_GEN_SFYPRO}, {"VOICE_GEN_SHFYPRO", VOICE_GEN_SHFYPRO}, {"VOICE_GEN_SHMYCR", VOICE_GEN_SHMYCR}, {"VOICE_GEN_SMYST", VOICE_GEN_SMYST}, {"VOICE_GEN_SMYST2", VOICE_GEN_SMYST2}, {"VOICE_GEN_SOFORI", VOICE_GEN_SOFORI}, {"VOICE_GEN_SOFOST", VOICE_GEN_SOFOST}, {"VOICE_GEN_SOFYBU", VOICE_GEN_SOFYBU}, {"VOICE_GEN_SOFYRI", VOICE_GEN_SOFYRI}, {"VOICE_GEN_SOFYST", VOICE_GEN_SOFYST}, {"VOICE_GEN_SOMOBU", VOICE_GEN_SOMOBU}, {"VOICE_GEN_SOMORI", VOICE_GEN_SOMORI}, {"VOICE_GEN_SOMOST", VOICE_GEN_SOMOST}, {"VOICE_GEN_SOMYAP", VOICE_GEN_SOMYAP}, {"VOICE_GEN_SOMYBU", VOICE_GEN_SOMYBU}, {"VOICE_GEN_SOMYRI", VOICE_GEN_SOMYRI}, {"VOICE_GEN_SOMYST", VOICE_GEN_SOMYST}, {"VOICE_GEN_SWFOPRO", VOICE_GEN_SWFOPRO}, {"VOICE_GEN_SWFORI", VOICE_GEN_SWFORI}, {"VOICE_GEN_SWFOST", VOICE_GEN_SWFOST}, {"VOICE_GEN_SWFYRI", VOICE_GEN_SWFYRI}, {"VOICE_GEN_SWFYST", VOICE_GEN_SWFYST}, {"VOICE_GEN_SWFYSTR", VOICE_GEN_SWFYSTR}, {"VOICE_GEN_SWMOCD", VOICE_GEN_SWMOCD}, {"VOICE_GEN_SWMORI", VOICE_GEN_SWMORI}, {"VOICE_GEN_SWMOST", VOICE_GEN_SWMOST}, {"VOICE_GEN_SWMOTR1", VOICE_GEN_SWMOTR1}, {"VOICE_GEN_SWMOTR2", VOICE_GEN_SWMOTR2}, {"VOICE_GEN_SWMOTR3", VOICE_GEN_SWMOTR3}, {"VOICE_GEN_SWMOTR4", VOICE_GEN_SWMOTR4}, {"VOICE_GEN_SWMOTR5", VOICE_GEN_SWMOTR5}, {"VOICE_GEN_SWMYCR", VOICE_GEN_SWMYCR}, {"VOICE_GEN_SWMYHP1", VOICE_GEN_SWMYHP1}, {"VOICE_GEN_SWMYHP2", VOICE_GEN_SWMYHP2}, {"VOICE_GEN_SWMYRI", VOICE_GEN_SWMYRI}, {"VOICE_GEN_SWMYST", VOICE_GEN_SWMYST}, {"VOICE_GEN_VBFYPRO", VOICE_GEN_VBFYPRO}, {"VOICE_GEN_VBFYST2", VOICE_GEN_VBFYST2}, {"VOICE_GEN_VBMOCD", VOICE_GEN_VBMOCD}, {"VOICE_GEN_VBMYCR", VOICE_GEN_VBMYCR}, {"VOICE_GEN_VBMYELV", VOICE_GEN_VBMYELV}, {"VOICE_GEN_VHFYPRO", VOICE_GEN_VHFYPRO}, {"VOICE_GEN_VHFYST3", VOICE_GEN_VHFYST3}, {"VOICE_GEN_VHMYCR", VOICE_GEN_VHMYCR}, {"VOICE_GEN_VHMYELV", VOICE_GEN_VHMYELV}, {"VOICE_GEN_VIMYELV", VOICE_GEN_VIMYELV}, {"VOICE_GEN_VWFYPRO", VOICE_GEN_VWFYPRO}, {"VOICE_GEN_VWFYST1", VOICE_GEN_VWFYST1}, {"VOICE_GEN_VWFYWAI", VOICE_GEN_VWFYWAI}, {"VOICE_GEN_VWMOTR1", VOICE_GEN_VWMOTR1}, {"VOICE_GEN_VWMOTR2", VOICE_GEN_VWMOTR2}, {"VOICE_GEN_VWMYAP", VOICE_GEN_VWMYAP}, {"VOICE_GEN_VWMYBJD", VOICE_GEN_VWMYBJD}, {"VOICE_GEN_VWMYCD", VOICE_GEN_VWMYCD}, {"VOICE_GEN_VWMYCR", VOICE_GEN_VWMYCR}, {"VOICE_GEN_WFOPJ", VOICE_GEN_WFOPJ}, {"VOICE_GEN_WFORI", VOICE_GEN_WFORI}, {"VOICE_GEN_WFOST", VOICE_GEN_WFOST}, {"VOICE_GEN_WFYBE", VOICE_GEN_WFYBE}, {"VOICE_GEN_WFYBU", VOICE_GEN_WFYBU}, {"VOICE_GEN_WFYCRK", VOICE_GEN_WFYCRK}, {"VOICE_GEN_WFYCRP", VOICE_GEN_WFYCRP}, {"VOICE_GEN_WFYJG", VOICE_GEN_WFYJG}, {"VOICE_GEN_WFYLG", VOICE_GEN_WFYLG}, {"VOICE_GEN_WFYPRO", VOICE_GEN_WFYPRO}, {"VOICE_GEN_WFYRI", VOICE_GEN_WFYRI}, {"VOICE_GEN_WFYRO", VOICE_GEN_WFYRO}, {"VOICE_GEN_WFYST", VOICE_GEN_WFYST}, {"VOICE_GEN_WFYSTEW", VOICE_GEN_WFYSTEW}, {"VOICE_GEN_WMOMIB", VOICE_GEN_WMOMIB}, {"VOICE_GEN_WMOPJ", VOICE_GEN_WMOPJ}, {"VOICE_GEN_WMOPREA", VOICE_GEN_WMOPREA}, {"VOICE_GEN_WMORI", VOICE_GEN_WMORI}, {"VOICE_GEN_WMOSCI", VOICE_GEN_WMOSCI}, {"VOICE_GEN_WMOST", VOICE_GEN_WMOST}, {"VOICE_GEN_WMOTR1", VOICE_GEN_WMOTR1}, {"VOICE_GEN_WMYBE", VOICE_GEN_WMYBE}, {"VOICE_GEN_WMYBMX", VOICE_GEN_WMYBMX}, {"VOICE_GEN_WMYBOUN", VOICE_GEN_WMYBOUN}, {"VOICE_GEN_WMYBOX", VOICE_GEN_WMYBOX}, {"VOICE_GEN_WMYBP", VOICE_GEN_WMYBP}, {"VOICE_GEN_WMYBU", VOICE_GEN_WMYBU}, {"VOICE_GEN_WMYCD1", VOICE_GEN_WMYCD1}, {"VOICE_GEN_WMYCD2", VOICE_GEN_WMYCD2}, {"VOICE_GEN_WMYCH", VOICE_GEN_WMYCH}, {"VOICE_GEN_WMYCON", VOICE_GEN_WMYCON}, {"VOICE_GEN_WMYCONB", VOICE_GEN_WMYCONB}, {"VOICE_GEN_WMYCR", VOICE_GEN_WMYCR}, {"VOICE_GEN_WMYDRUG", VOICE_GEN_WMYDRUG}, {"VOICE_GEN_WMYGAR", VOICE_GEN_WMYGAR}, {"VOICE_GEN_WMYGOL1", VOICE_GEN_WMYGOL1}, {"VOICE_GEN_WMYGOL2", VOICE_GEN_WMYGOL2}, {"VOICE_GEN_WMYJG", VOICE_GEN_WMYJG}, {"VOICE_GEN_WMYLG", VOICE_GEN_WMYLG}, {"VOICE_GEN_WMYMECH", VOICE_GEN_WMYMECH}, {"VOICE_GEN_WMYMOUN", VOICE_GEN_WMYMOUN}, {"VOICE_GEN_WMYPLT", VOICE_GEN_WMYPLT}, {"VOICE_GEN_WMYRI", VOICE_GEN_WMYRI}, {"VOICE_GEN_WMYRO", VOICE_GEN_WMYRO}, {"VOICE_GEN_WMYSGRD", VOICE_GEN_WMYSGRD}, {"VOICE_GEN_WMYSKAT", VOICE_GEN_WMYSKAT}, {"VOICE_GEN_WMYST", VOICE_GEN_WMYST}, {"VOICE_GEN_WMYTX1", VOICE_GEN_WMYTX1}, {"VOICE_GEN_WMYTX2", VOICE_GEN_WMYTX2}, {"VOICE_GEN_WMYVA", VOICE_GEN_WMYVA}
        });
        return notsa::find_value_or(mapping, name, -1);
    }
    case PED_TYPE_EMG: {
        static const auto mapping = notsa::make_mapping<std::string_view, eEmgSpeechVoices>({
            {"VOICE_EMG_ARMY1", VOICE_EMG_ARMY1}, {"VOICE_EMG_ARMY2", VOICE_EMG_ARMY2}, {"VOICE_EMG_ARMY3", VOICE_EMG_ARMY3}, {"VOICE_EMG_EMT1", VOICE_EMG_EMT1}, {"VOICE_EMG_EMT2", VOICE_EMG_EMT2}, {"VOICE_EMG_EMT3", VOICE_EMG_EMT3}, {"VOICE_EMG_EMT4", VOICE_EMG_EMT4}, {"VOICE_EMG_EMT5", VOICE_EMG_EMT5}, {"VOICE_EMG_FBI2", VOICE_EMG_FBI2}, {"VOICE_EMG_FBI3", VOICE_EMG_FBI3}, {"VOICE_EMG_FBI4", VOICE_EMG_FBI4}, {"VOICE_EMG_FBI5", VOICE_EMG_FBI5}, {"VOICE_EMG_FBI6", VOICE_EMG_FBI6}, {"VOICE_EMG_LAPD1", VOICE_EMG_LAPD1}, {"VOICE_EMG_LAPD2", VOICE_EMG_LAPD2}, {"VOICE_EMG_LAPD3", VOICE_EMG_LAPD3}, {"VOICE_EMG_LAPD4", VOICE_EMG_LAPD4}, {"VOICE_EMG_LAPD5", VOICE_EMG_LAPD5}, {"VOICE_EMG_LAPD6", VOICE_EMG_LAPD6}, {"VOICE_EMG_LAPD7", VOICE_EMG_LAPD7}, {"VOICE_EMG_LAPD8", VOICE_EMG_LAPD8}, {"VOICE_EMG_LVPD1", VOICE_EMG_LVPD1}, {"VOICE_EMG_LVPD2", VOICE_EMG_LVPD2}, {"VOICE_EMG_LVPD3", VOICE_EMG_LVPD3}, {"VOICE_EMG_LVPD4", VOICE_EMG_LVPD4}, {"VOICE_EMG_LVPD5", VOICE_EMG_LVPD5}, {"VOICE_EMG_MCOP1", VOICE_EMG_MCOP1}, {"VOICE_EMG_MCOP2", VOICE_EMG_MCOP2}, {"VOICE_EMG_MCOP3", VOICE_EMG_MCOP3}, {"VOICE_EMG_MCOP4", VOICE_EMG_MCOP4}, {"VOICE_EMG_MCOP5", VOICE_EMG_MCOP5}, {"VOICE_EMG_MCOP6", VOICE_EMG_MCOP6}, {"VOICE_EMG_PULASKI", VOICE_EMG_PULASKI}, {"VOICE_EMG_RCOP1", VOICE_EMG_RCOP1}, {"VOICE_EMG_RCOP2", VOICE_EMG_RCOP2}, {"VOICE_EMG_RCOP3", VOICE_EMG_RCOP3}, {"VOICE_EMG_RCOP4", VOICE_EMG_RCOP4}, {"VOICE_EMG_SFPD1", VOICE_EMG_SFPD1}, {"VOICE_EMG_SFPD2", VOICE_EMG_SFPD2}, {"VOICE_EMG_SFPD3", VOICE_EMG_SFPD3}, {"VOICE_EMG_SFPD4", VOICE_EMG_SFPD4}, {"VOICE_EMG_SFPD5", VOICE_EMG_SFPD5}, {"VOICE_EMG_SWAT1", VOICE_EMG_SWAT1}, {"VOICE_EMG_SWAT2", VOICE_EMG_SWAT2}, {"VOICE_EMG_SWAT4", VOICE_EMG_SWAT4}, {"VOICE_EMG_SWAT6", VOICE_EMG_SWAT6},
        });
        return notsa::find_value_or(mapping, name, -1);
    }
    case PED_TYPE_PLAYER: {
        static const auto mapping = notsa::make_mapping<std::string_view, ePlySpeechVoices>({
            {"VOICE_PLY_AG", VOICE_PLY_AG}, {"VOICE_PLY_AG2", VOICE_PLY_AG2}, {"VOICE_PLY_AR", VOICE_PLY_AR}, {"VOICE_PLY_AR2", VOICE_PLY_AR2}, {"VOICE_PLY_CD", VOICE_PLY_CD}, {"VOICE_PLY_CD2", VOICE_PLY_CD2}, {"VOICE_PLY_CF", VOICE_PLY_CF}, {"VOICE_PLY_CF2", VOICE_PLY_CF2}, {"VOICE_PLY_CG", VOICE_PLY_CG}, {"VOICE_PLY_CG2", VOICE_PLY_CG2}, {"VOICE_PLY_CR", VOICE_PLY_CR}, {"VOICE_PLY_CR2", VOICE_PLY_CR2}, {"VOICE_PLY_PG", VOICE_PLY_PG}, {"VOICE_PLY_PG2", VOICE_PLY_PG2}, {"VOICE_PLY_PR", VOICE_PLY_PR}, {"VOICE_PLY_PR2", VOICE_PLY_PR2}, {"VOICE_PLY_WG", VOICE_PLY_WG}, {"VOICE_PLY_WG2", VOICE_PLY_WG2}, {"VOICE_PLY_WR", VOICE_PLY_WR}, {"VOICE_PLY_WR2", VOICE_PLY_WR2},
        });
        return notsa::find_value_or(mapping, name, -1);
    }
    case PED_TYPE_GANG: {
        static const auto mapping = notsa::make_mapping<std::string_view, eGngSpeechVoices>({
            {"VOICE_GNG_BALLAS1", VOICE_GNG_BALLAS1}, {"VOICE_GNG_BALLAS2", VOICE_GNG_BALLAS2}, {"VOICE_GNG_BALLAS3", VOICE_GNG_BALLAS3}, {"VOICE_GNG_BALLAS4", VOICE_GNG_BALLAS4}, {"VOICE_GNG_BALLAS5", VOICE_GNG_BALLAS5}, {"VOICE_GNG_BIG_BEAR", VOICE_GNG_BIG_BEAR}, {"VOICE_GNG_CESAR", VOICE_GNG_CESAR}, {"VOICE_GNG_DNB1", VOICE_GNG_DNB1}, {"VOICE_GNG_DNB2", VOICE_GNG_DNB2}, {"VOICE_GNG_DNB3", VOICE_GNG_DNB3}, {"VOICE_GNG_DNB5", VOICE_GNG_DNB5}, {"VOICE_GNG_DWAINE", VOICE_GNG_DWAINE}, {"VOICE_GNG_FAM1", VOICE_GNG_FAM1}, {"VOICE_GNG_FAM2", VOICE_GNG_FAM2}, {"VOICE_GNG_FAM3", VOICE_GNG_FAM3}, {"VOICE_GNG_FAM4", VOICE_GNG_FAM4}, {"VOICE_GNG_FAM5", VOICE_GNG_FAM5}, {"VOICE_GNG_JIZZY", VOICE_GNG_JIZZY}, {"VOICE_GNG_LSV1", VOICE_GNG_LSV1}, {"VOICE_GNG_LSV2", VOICE_GNG_LSV2}, {"VOICE_GNG_LSV3", VOICE_GNG_LSV3}, {"VOICE_GNG_LSV4", VOICE_GNG_LSV4}, {"VOICE_GNG_LSV5", VOICE_GNG_LSV5}, {"VOICE_GNG_MACCER", VOICE_GNG_MACCER}, {"VOICE_GNG_MAFBOSS", VOICE_GNG_MAFBOSS}, {"VOICE_GNG_OGLOC", VOICE_GNG_OGLOC}, {"VOICE_GNG_RYDER", VOICE_GNG_RYDER}, {"VOICE_GNG_SFR1", VOICE_GNG_SFR1}, {"VOICE_GNG_SFR2", VOICE_GNG_SFR2}, {"VOICE_GNG_SFR3", VOICE_GNG_SFR3}, {"VOICE_GNG_SFR4", VOICE_GNG_SFR4}, {"VOICE_GNG_SFR5", VOICE_GNG_SFR5}, {"VOICE_GNG_SMOKE", VOICE_GNG_SMOKE}, {"VOICE_GNG_STRI1", VOICE_GNG_STRI1}, {"VOICE_GNG_STRI2", VOICE_GNG_STRI2}, {"VOICE_GNG_STRI4", VOICE_GNG_STRI4}, {"VOICE_GNG_STRI5", VOICE_GNG_STRI5}, {"VOICE_GNG_SWEET", VOICE_GNG_SWEET}, {"VOICE_GNG_TBONE", VOICE_GNG_TBONE}, {"VOICE_GNG_TORENO", VOICE_GNG_TORENO}, {"VOICE_GNG_TRUTH", VOICE_GNG_TRUTH}, {"VOICE_GNG_VLA1", VOICE_GNG_VLA1}, {"VOICE_GNG_VLA2", VOICE_GNG_VLA2}, {"VOICE_GNG_VLA3", VOICE_GNG_VLA3}, {"VOICE_GNG_VLA4", VOICE_GNG_VLA4}, {"VOICE_GNG_VLA5", VOICE_GNG_VLA5}, {"VOICE_GNG_VMAFF1", VOICE_GNG_VMAFF1}, {"VOICE_GNG_VMAFF2", VOICE_GNG_VMAFF2}, {"VOICE_GNG_VMAFF3", VOICE_GNG_VMAFF3}, {"VOICE_GNG_VMAFF4", VOICE_GNG_VMAFF4}, {"VOICE_GNG_VMAFF5", VOICE_GNG_VMAFF5}, {"VOICE_GNG_WOOZIE", VOICE_GNG_WOOZIE},
        });
        return notsa::find_value_or(mapping, name, -1);
    }
    case PED_TYPE_GFD: {
        static const auto mapping = notsa::make_mapping<std::string_view, eGfdSpeechVoices>({
            {"VOICE_GFD_BARBARA", VOICE_GFD_BARBARA}, {"VOICE_GFD_BMOBAR", VOICE_GFD_BMOBAR}, {"VOICE_GFD_BMYBARB", VOICE_GFD_BMYBARB}, {"VOICE_GFD_BMYTATT", VOICE_GFD_BMYTATT}, {"VOICE_GFD_CATALINA", VOICE_GFD_CATALINA}, {"VOICE_GFD_DENISE", VOICE_GFD_DENISE}, {"VOICE_GFD_HELENA", VOICE_GFD_HELENA}, {"VOICE_GFD_KATIE", VOICE_GFD_KATIE}, {"VOICE_GFD_MICHELLE", VOICE_GFD_MICHELLE}, {"VOICE_GFD_MILLIE", VOICE_GFD_MILLIE}, {"VOICE_GFD_POL_ANN", VOICE_GFD_POL_ANN}, {"VOICE_GFD_WFYBURG", VOICE_GFD_WFYBURG}, {"VOICE_GFD_WFYCLOT", VOICE_GFD_WFYCLOT}, {"VOICE_GFD_WMYAMMO", VOICE_GFD_WMYAMMO}, {"VOICE_GFD_WMYBARB", VOICE_GFD_WMYBARB}, {"VOICE_GFD_WMYBELL", VOICE_GFD_WMYBELL}, {"VOICE_GFD_WMYCLOT", VOICE_GFD_WMYCLOT}, {"VOICE_GFD_WMYPIZZ", VOICE_GFD_WMYPIZZ},
        });
        return notsa::find_value_or(mapping, name, -1);
    }
    default:
        NOTSA_UNREACHABLE();
    }
}

// 0x4E3EB0
void CAEPedSpeechAudioEntity::DisableAllPedSpeech() {
    s_bAllSpeechDisabled = true;
}

// 0x4E44F0
bool __stdcall CAEPedSpeechAudioEntity::IsGlobalContextPain(eGlobalSpeechContext gCtx) {
    return CTX_GLOBAL_PAIN_START < gCtx && gCtx < CTX_GLOBAL_PAIN_END;
}

// 0x4E3ED0
void CAEPedSpeechAudioEntity::SetCJMood(eCJMood basicMood, uint32 overrideTimeMS, int16 isGangBanging, int16 isFat, int16 isWellDressed) {
    s_nCJMoodOverrideTime = CTimer::GetTimeInMS() + overrideTimeMS;
    s_nCJBasicMood  = [=]{
        switch (basicMood) {
        case MOOD_AR:
        case MOOD_CR:
        case MOOD_PR:
        case MOOD_WR: return basicMood;
        default:      return MOOD_UNK;
        }
    }();
    s_nCJGangBanging = isGangBanging;
    s_nCJFat         = isFat;
    s_nCJWellDressed = isWellDressed;
}

// 0x4E3EC0
void CAEPedSpeechAudioEntity::EnableAllPedSpeech() {
    s_bAllSpeechDisabled = false;
}

// 0x4E4270
bool CAEPedSpeechAudioEntity::IsCJDressedInForGangSpeech() {
    static constexpr struct {
        eClothesTexturePart ClothesPart;
        const char*         TexName;
    } GANG_SPEECH_CLOTHES[] = {
        { CLOTHES_TEXTURE_TORSO,   "hoodyagreen" },
        { CLOTHES_TEXTURE_TORSO,   "shirtbgang"  },
        { CLOTHES_TEXTURE_LEGS,    "tracktrgang" },
        { CLOTHES_TEXTURE_LEGS,    "denimsgang"  },
        { CLOTHES_TEXTURE_HATS,    "bandgang"    },
        { CLOTHES_TEXTURE_HATS,    "hockey"      },
        { CLOTHES_TEXTURE_HATS,    "capgang"     },
        { CLOTHES_TEXTURE_HATS,    "capgangback" },
        { CLOTHES_TEXTURE_HATS,    "capgangside" },
        { CLOTHES_TEXTURE_HATS,    "capgangover" },
        { CLOTHES_TEXTURE_HATS,    "capgangup"   },
        { CLOTHES_TEXTURE_GLASSES, "bandred3"    },
        { CLOTHES_TEXTURE_GLASSES, "bandblue3"   },
        { CLOTHES_TEXTURE_GLASSES, "bandgang3"   },
        { CLOTHES_TEXTURE_GLASSES, "bandblack3"  },
    };

    if (!FindPlayerPed(PED_TYPE_PLAYER1)) {
        return false;
    }

    if (CGameLogic::FindCityClosestToPoint(FindPlayerCoors()) != LEVEL_NAME_LOS_SANTOS) {
        return false;
    }

    return rng::any_of(GANG_SPEECH_CLOTHES, [pcd = FindPlayerPed()->GetClothesDesc()](auto& gc) {
        return pcd->m_anTextureKeys[gc.ClothesPart] == CKeyGen::GetUppercaseKey(gc.TexName);
    });
}

// 0x4E4600
bool __stdcall CAEPedSpeechAudioEntity::IsGlobalContextImportantForInterupting(eGlobalSpeechContext gCtx) {
    return GetGlobalSpeechContextInfo(gCtx)->IsImportantForInterrupting; // NOTSA: Use context info lookup instead of static switch
    /*
    switch (gCtx) {
    case CTX_GLOBAL_ARREST:
    case CTX_GLOBAL_ARRESTED:
    case CTX_GLOBAL_JOIN_GANG_NO:
    case CTX_GLOBAL_JOIN_GANG_YES:
    case CTX_GLOBAL_JOIN_ME_ASK:
    return true;
    default:
    return false;
    }
    */
}

// 0x4E46F0 - unused
bool CAEPedSpeechAudioEntity::IsGlobalContextUberImportant(int16 gCtx) {
    return false;
}

// 0x4E46B0
bool CAEPedSpeechAudioEntity::IsGlobalContextImportantForWidescreen(eGlobalSpeechContext gCtx) {
    switch (m_PedAudioType) {
    case PED_TYPE_GFD:
    case PED_TYPE_PLAYER:
        return true;
    }
    return GetGlobalSpeechContextInfo(gCtx)->IsImportantForWidescreen; // NOTSA: Use context info lookup instead of static switch
    /*
    switch (gCtx) {
    case CTX_GLOBAL_ARREST:
    case CTX_GLOBAL_ARRESTED:
    case CTX_GLOBAL_HAVING_SEX:
        return true;
    }
    return false;
    */
}

// 0x4E4260
int8 CAEPedSpeechAudioEntity::GetSexForSpecialPed(uint32 a1) {
    return 1;
}

// 0x4E47E0
int16 CAEPedSpeechAudioEntity::GetRepeatTime(eGlobalSpeechContext gCtx) {
    assert(gCtx < CTX_GLOBAL_NUM); // OG: return 0

    if (const auto* const ctxi = GetGlobalSpeechContextInfo(gCtx)) {
        return ctxi->RepeatTime;
    }
    return 0;
}

// 0x4E4840
void CAEPedSpeechAudioEntity::LoadAndPlaySpeech(uint32 playbackTimeOffsetMS) {
    auto& ss = GetCurrentSpeech();
    switch (ss.Status) {
    case CAEPedSpeechSlot::eStatus::FREE:
    case CAEPedSpeechSlot::eStatus::RESERVED:
        break;
    default:
        return;
    }

    AEAudioHardware.LoadSound(m_BankID, m_SoundID, SND_BANK_SLOT_SPEECH1 + m_PedSpeechSlotID); // TODO: Helper
    ss.Status            = CAEPedSpeechSlot::eStatus::LOADING;
    ss.SoundBankID       = m_BankID;
    ss.SoundID           = m_SoundID;
    ss.AudioEntity       = this;
    ss.StartPlaybackTime = CTimer::GetTimeInMS() + playbackTimeOffsetMS;
    ss.PedAudioType      = m_PedAudioType;
    ss.GCtx              = m_LastGCtx;
    ss.ForceAudible      = m_IsForcedAudible;
}

// 0x4E49B0
int32 CAEPedSpeechAudioEntity::GetNumSlotsPlayingContext(int16 context) {
    return rng::count_if(s_PedSpeechSlots, [&](CAEPedSpeechSlot& speech) {
        return speech.Status != CAEPedSpeechSlot::eStatus::FREE && speech.GCtx == context;
    });
}

// 0x4E49E0
uint32 CAEPedSpeechAudioEntity::GetNextPlayTime(eGlobalSpeechContext gCtx) {
    assert(gCtx < CTX_GLOBAL_NUM); // OG: `return 0;`

    return GetNextPlayTimeRef(gCtx);
}

// 0x4E4A20
void CAEPedSpeechAudioEntity::SetNextPlayTime(eGlobalSpeechContext gCtx) {
    assert(gCtx < CTX_GLOBAL_NUM); // OG: `return;`

    if (const auto* const ctxi = GetGlobalSpeechContextInfo(gCtx)) {
        GetNextPlayTimeRef(gCtx) = CTimer::GetTimeInMS() + ctxi->RepeatTime + CAEAudioUtility::GetRandomNumberInRange(1, 1000);
    }
}

// 0x4E56D0
void CAEPedSpeechAudioEntity::DisablePedSpeech(int16 a1) {
    if (m_IsInitialized) {
        m_IsSpeechDisabled = true;
        if (a1)
            StopCurrentSpeech();
    }
}

// 0x4E5700
void CAEPedSpeechAudioEntity::DisablePedSpeechForScriptSpeech(int16 a1) {
    if (m_IsInitialized) {
        m_IsSpeechForScriptsDisabled = true;
        if (a1)
            StopCurrentSpeech();
    }
}

// 0x4E5730
bool CAEPedSpeechAudioEntity::CanPedSayGlobalContext(eGlobalSpeechContext gCtx) const {
    if (!m_IsInitialized) {
        return false;
    }
    const auto sCtx = GetSpecificSpeechContext(gCtx, m_PedAudioType);
    if (sCtx == -1) {
        return false;
    }
    if (const auto* const ctxi = GetSpecificSpeechContextInfo(sCtx, gCtx, m_PedAudioType, m_VoiceID)) {
        return ctxi->FirstSoundID != -1;
    }
    return false;
}

// 0x4E58C0
int8 CAEPedSpeechAudioEntity::GetVoiceAndTypeFromModel(eModelID modelId) {
    auto* const mi = CModelInfo::GetModelInfo(modelId)->AsPedModelInfoPtr();
    if (mi->m_nPedAudioType == -1 || mi->m_nPedAudioType >= PED_TYPE_NUM) {
        return 0;
    }

    if (mi->m_nPedAudioType == PED_TYPE_SPC) {
        return GetVoiceAndTypeForSpecialPed(mi->m_nKey);
    }

    m_VoiceID = mi->m_nVoiceId;
    if (m_VoiceID == -1) {
        return 0;
    }

    mi->IncrementVoice();

    return 1;
}

// 0x4E5920
int16 CAEPedSpeechAudioEntity::GetSoundAndBankIDs(eGlobalSpeechContext gCtx, eSpecificSpeechContext& outSpecificSpeechContext) {
    // Left out some ifs error checking ifs as they aren't necessary (They're gonna get caught in debug anyways)

    const auto sCtx = GetSpecificSpeechContext(gCtx, m_PedAudioType);
    if (sCtx == -1) {
        return -1;
    }
    outSpecificSpeechContext = sCtx;

    const auto* const ctx = GetSpecificSpeechContextInfo(sCtx, gCtx, m_PedAudioType, m_VoiceID);
    if (!ctx) {
        return -1;
    }

    m_BankID = GetVoiceSoundBank(gCtx, m_PedAudioType, m_VoiceID);

    if (ctx->FirstSoundID == -1) {
        return -1;
    }
    assert(ctx->FirstSoundID <= ctx->LastSoundID);

    const size_t numSounds = ctx->LastSoundID - ctx->FirstSoundID;
    assert(numSounds > 0);

    // Find sound ID to use
    // If all possible sounds are in the PhraseMemory,
    // the least recently used one will be picked
    // NOTE: Below is a better version of what they did (without using an intermediary array)

    const auto GetPhraseIndexInMemory = [this](int32 soundID) -> int16 {
        for (auto&& [i, p] : notsa::enumerate(s_PhraseMemory)) {
            if (p.SoundID == soundID && p.BankID == m_BankID) {
                return i;
            }
        }
        return -1;
    };

    int16      soundIDToUse    = -1;
    int16      maxPhraseIMemdx = -1;
    const auto rndOffset    = CAEAudioUtility::GetRandomNumberInRange(0u, numSounds);
    for (size_t i = 0; i < std::min(20u, numSounds); i++) { // NOTE: Here you can tune the maximum tries (Default: 20)
        const auto soundIDCurr  = ctx->FirstSoundID + (int16)((rndOffset + i) % (numSounds + 1));
        const auto phraseMemIdx = GetPhraseIndexInMemory(soundIDCurr);

        // Not in memory at all? Good, pick this!
        if (phraseMemIdx == -1) {
            soundIDToUse = soundIDCurr;
            break;
        }

        // Maybe this was used a longer time ago than the previous one?
        if (maxPhraseIMemdx < phraseMemIdx) {
            soundIDToUse = soundIDCurr;
            maxPhraseIMemdx = phraseMemIdx;
        }
    }
    assert(soundIDToUse != -1);

    // Insert into front of `s_PhraseMemory`
    rng::shift_right(s_PhraseMemory, 1);
    s_PhraseMemory[0] = {soundIDToUse, m_BankID};

    return soundIDToUse - ctx->FirstSoundID;
}

// 0x4E5F10
int16 CAEPedSpeechAudioEntity::CanWePlayGlobalSpeechContext(eGlobalSpeechContext gCtx) {
    if (!IsGlobalContextImportantForInterupting(gCtx) && !IsGlobalContextPain(gCtx)) {
        const auto CheckSlot = [](auto slot) {
            return s_PedSpeechSlots[slot].Status == CAEPedSpeechSlot::eStatus::RESERVED
                ? slot
                : -1;
        };
        if (s_bPedConversationHappening) {
            if (this == &s_pConversationPed1->m_pedSpeech) {
                return CheckSlot(s_pConversationPedSlot1);
            }
            if (this == &s_pConversationPed2->m_pedSpeech) {
                return CheckSlot(s_pConversationPedSlot2);
            }
        } else if (s_bPlayerConversationHappening) {
            if (this == &s_pPlayerConversationPed->m_pedSpeech) {
                return CheckSlot(s_pConversationPedSlot1);
            }    
        }
    }
    return GetNextPlayTime(gCtx) < CTimer::GetTimeInMS()
        ? GetFreeSpeechSlot() // s_NextSpeechSlot is set in `GetFreeSpeechSlot`
        : -1;
}

// 0x4E6550
int16 CAEPedSpeechAudioEntity::AddSayEvent(eAudioEvents audioEvent, eGlobalSpeechContext gCtx, uint32 startTimeDelayMs, float probability, bool overrideSilence, bool isForceAudible, bool isFrontEnd) {
    // tempTimeOffset == 0, so ignore it

    if (!m_IsInitialized || s_bAllSpeechDisabled || m_IsSpeechForScriptsDisabled) {
        return -1;
    }

    if (TheCamera.m_bWideScreenOn && !IsGlobalContextImportantForWidescreen(gCtx)) {
        return -1;
    }

    if (CGameLogic::GameState == eGameState::GAME_STATE_PLAYING_LOGO) {
        return -1;
    }

    if (gCtx == CTX_GLOBAL_CHAT) {
        if (CTimer::GetFrameCounter() % 64 || s_bPedConversationHappening || s_bPlayerConversationHappening) {
            return -1;
        }
    } else if (gCtx == CTX_GLOBAL_PAIN_CJ_DROWNING && CWeather::UnderWaterness < 0.5f)  {
        return -1;
    }

    if (!CAEAudioUtility::ResolveProbability(probability) || audioEvent != AE_SPEECH_PED) {
        return -1;
    }

    if (m_IsSpeechDisabled && !overrideSilence) {
        return -1;
    }

    const auto IsContextImportant = [](eGlobalSpeechContext c) {
        return IsGlobalContextPain(c) || IsGlobalContextImportantForInterupting(c);
    };
    if (IsContextImportant(gCtx)) {
        if (m_IsPlayingSpeech && IsContextImportant(GetCurrentSpeech().GCtx)) {
            return -1;
        }
        StopCurrentSpeech();
    }

    if (!m_pEntity->AsPed()->IsAlive() && !IsGlobalContextPain(gCtx)) {
        return -1;
    }

    if (IsGlobalContextImportantForStreaming(gCtx) || isForceAudible || isFrontEnd) {
        if (CStreaming::ms_numModelsRequested > 15 && CAEVehicleAudioEntity::s_pVehicleAudioSettingsForRadio) {
            return -1;
        }
    } else if (CStreaming::IsVeryBusy()) {
        return -1;
    }

    if (m_PedAudioType == PED_TYPE_PLAYER) {
        if (FindPlayerPed() != m_pEntity || s_bAPlayerSpeaking && !m_IsPlayingSpeech) {
            return -1;
        }
    }

    if (CVector::DistSqr(TheCamera.GetPosition(), m_pEntity->GetPosition()) >= sq(40.f)) {
        return -1;
    }

    eSpecificSpeechContext sCtx;
    const auto soundID = CAEPedSpeechAudioEntity::GetSoundAndBankIDs(gCtx, sCtx);
    if (soundID == -1) {
        return -1;
    }

    const auto speechSlotID = m_PedAudioType == PED_TYPE_PLAYER
        ? CTimer::GetTimeInMS() >= GetNextPlayTime(gCtx)
            ? 5
            : -1
        : CAEPedSpeechAudioEntity::CanWePlayGlobalSpeechContext(gCtx);
    if (speechSlotID == -1) {
        return -1;
    }

    SetNextPlayTime(gCtx);
    m_IsFrontend = isFrontEnd;
    m_IsForcedAudible = isForceAudible;
    if (const auto veh = m_pEntity->AsPed()->GetVehicleIfInOne()) {
        m_IsForcedAudible |= veh == FindPlayerVehicle() && gCtx != CTX_GLOBAL_STOMACH_RUMBLE;
    }
    
    m_EventVolume = GetDefaultVolume(AE_SPEECH_PED);
    if (m_PedAudioType == PED_TYPE_PLAYER) {
        m_EventVolume += 3.f;
    }
    m_EventVolume += GetSpeechContextVolumeOffset(gCtx);

    m_IsPlayingSpeech = true;
    if (m_PedAudioType == PED_TYPE_PLAYER) {
        s_bAPlayerSpeaking = true;
    }

    m_LastGCtx        = gCtx;
    m_PedSpeechSlotID = speechSlotID;
    LoadAndPlaySpeech(startTimeDelayMs);

    return soundID;
}

// 0x4E68D0
void CAEPedSpeechAudioEntity::Initialise(CEntity* ped) {
    *this = CAEPedSpeechAudioEntity{ped->AsPed()};
}

// 0x4E69E0
bool CAEPedSpeechAudioEntity::CanPedHoldConversation() {
    return CanPedSayGlobalContext(CTX_GLOBAL_PCONV_QUESTION);
}

// 0x4E4510
bool CAEPedSpeechAudioEntity::IsGlobalContextImportantForStreaming(eGlobalSpeechContext gCtx) {
    switch (m_PedAudioType) {
    case PED_TYPE_GFD:
    case PED_TYPE_PLAYER:
        return true;
    }
    return GetGlobalSpeechContextInfo(gCtx)->IsImportantForStreaming; // NOTSA: Use context info lookup instead of static switch
    /*
    switch (gCtx) {
    case CTX_GLOBAL_ARREST:
    case CTX_GLOBAL_ARRESTED:
    case CTX_GLOBAL_CONV_DISL_CAR:
    case CTX_GLOBAL_CONV_DISL_CLOTHES:
    case CTX_GLOBAL_CONV_DISL_HAIR:
    case CTX_GLOBAL_CONV_DISL_PHYS:
    case CTX_GLOBAL_CONV_DISL_SHOES:
    case CTX_GLOBAL_CONV_DISL_SMELL:
    case CTX_GLOBAL_CONV_DISL_TATTOO:
    case CTX_GLOBAL_CONV_DISL_WEATHER:
    case CTX_GLOBAL_CONV_IGNORED:
    case CTX_GLOBAL_CONV_LIKE_CAR:
    case CTX_GLOBAL_CONV_LIKE_CLOTHES:
    case CTX_GLOBAL_CONV_LIKE_HAIR:
    case CTX_GLOBAL_CONV_LIKE_PHYS:
    case CTX_GLOBAL_CONV_LIKE_SHOES:
    case CTX_GLOBAL_CONV_LIKE_SMELL:
    case CTX_GLOBAL_CONV_LIKE_TATTOO:
    case CTX_GLOBAL_CONV_LIKE_WEATHER:
    case CTX_GLOBAL_JOIN_GANG_NO:
    case CTX_GLOBAL_JOIN_GANG_YES:
    case CTX_GLOBAL_JOIN_ME_ASK:
    case CTX_GLOBAL_PCONV_AGREE_BAD:
    case CTX_GLOBAL_PCONV_AGREE_GOOD:
    case CTX_GLOBAL_PCONV_ANS_NO:
    case CTX_GLOBAL_PCONV_DISMISS:
    case CTX_GLOBAL_PCONV_GREET_FEM:
    case CTX_GLOBAL_PCONV_GREET_MALE:
    case CTX_GLOBAL_PCONV_PART_FEM:
    case CTX_GLOBAL_PCONV_PART_MALE:
    case CTX_GLOBAL_PCONV_QUESTION:
    case CTX_GLOBAL_PCONV_STATE_BAD:
    case CTX_GLOBAL_PCONV_STATE_GOOD:
        return true;
    }
    return false;
    */
}

// 0x4E3F70
void CAEPedSpeechAudioEntity::EnablePedSpeech() {
    if (m_IsInitialized)
        m_IsSpeechDisabled = false;
}

// 0x4E3F90
void CAEPedSpeechAudioEntity::EnablePedSpeechForScriptSpeech() {
    if (m_IsInitialized)
        m_IsSpeechForScriptsDisabled = false;
}

// 0x4E3FB0
void CAEPedSpeechAudioEntity::StopCurrentSpeech() {
    plugin::CallMethod<0x4E3FB0, CAEPedSpeechAudioEntity*>(this);
}

// 0x4E4400
int8 CAEPedSpeechAudioEntity::GetSoundAndBankIDsForScriptedSpeech(int32 a2) {
    return plugin::CallMethodAndReturn<int8, 0x4E4400, CAEPedSpeechAudioEntity*, int32>(this, a2);
}

// 0x4E4200
bool CAEPedSpeechAudioEntity::GetSexFromModel(eModelID model) {
    const auto* const mi = CModelInfo::GetModelInfo(model)->AsPedModelInfoPtr();
    assert(mi->m_nPedAudioType < PED_TYPE_NUM);
    if (mi->m_nPedAudioType == PED_TYPE_SPC) {
        return true;
    }
    m_IsFemale = notsa::contains({PED_TYPE_CIVFEMALE, PED_TYPE_PROSTITUTE}, mi->GetPedType());
    return true;
}

// 0x4E3F50
bool CAEPedSpeechAudioEntity::GetPedTalking() {
    return m_IsInitialized && m_IsPlayingSpeech;
}

// 0x4E4170
int8 CAEPedSpeechAudioEntity::GetVoiceAndTypeForSpecialPed(uint32 modelNameHash) {
    return plugin::CallMethodAndReturn<int8, 0x4E4170, CAEPedSpeechAudioEntity*, uint32>(this, modelNameHash);
}

ePainSpeechVoices CAEPedSpeechAudioEntity::GetPainVoice() const {
    if (m_PedAudioType == PED_TYPE_PLAYER) {
        return VOICE_PAIN_CARL;
    }
    return m_IsFemale
        ? VOICE_PAIN_FEMALE
        : VOICE_PAIN_MALE;
}

// 0x4E3520
void CAEPedSpeechAudioEntity::UpdateParameters(CAESound* sound, int16 curPlayPos) {
    plugin::CallMethod<0x4E3520, CAEPedSpeechAudioEntity*, CAESound*, int16>(this, sound, curPlayPos);
}

// 0x4E4F70
void CAEPedSpeechAudioEntity::AddScriptSayEvent(int32 a1, int32 a2, uint8 a3, uint8 a4, uint8 a5) {
    plugin::CallMethod<0x4E4F70, CAEPedSpeechAudioEntity*, int32, int32, uint8, uint8, uint8>(this, a1, a2, a3, a4, a5);
}

// 0x4E5670
void CAEPedSpeechAudioEntity::Terminate() {
    plugin::CallMethod<0x4E5670, CAEPedSpeechAudioEntity*>(this);
}

// 0x4E5CD0
void CAEPedSpeechAudioEntity::PlayLoadedSound() {
    plugin::CallMethod<0x4E5CD0, CAEPedSpeechAudioEntity*>(this);
}

// 0x4E4120
int16 CAEPedSpeechAudioEntity::GetAllocatedVoice() {
    return m_VoiceID;
}

// 0x4E5800
bool CAEPedSpeechAudioEntity::WillPedChatAboutTopic(int16 topic) {
    switch (topic) {
    case 0:  return CanPedSayGlobalContext(CTX_GLOBAL_CONV_DISL_CAR);
    case 1:  return CanPedSayGlobalContext(CTX_GLOBAL_CONV_DISL_CLOTHES);
    case 2:  return CanPedSayGlobalContext(CTX_GLOBAL_CONV_DISL_HAIR);
    case 3:  return CanPedSayGlobalContext(CTX_GLOBAL_CONV_DISL_PHYS);
    case 4:  return CanPedSayGlobalContext(CTX_GLOBAL_CONV_DISL_SHOES);
    case 5:  return CanPedSayGlobalContext(CTX_GLOBAL_CONV_DISL_SMELL);
    case 6:  return CanPedSayGlobalContext(CTX_GLOBAL_CONV_DISL_TATTOO);
    case 7:  return CanPedSayGlobalContext(CTX_GLOBAL_CONV_DISL_WEATHER);
    case 8:
    case 9:  return true;
    default: return false;
    }
}

// 0x4E4130
int16 CAEPedSpeechAudioEntity::GetPedType() {
    return m_IsInitialized
        ? m_PedAudioType
        : PED_TYPE_UNK;
}

// 0x4E4150
bool CAEPedSpeechAudioEntity::IsPedFemaleForAudio() {
    return m_IsInitialized
        ? m_IsFemale
        : false;
}

// notsa
int32 CAEPedSpeechAudioEntity::GetFreeSpeechSlot() {
    const auto size = s_PedSpeechSlots.size() - 1;
    for (size_t n = 0; n < size; n++) {
        const auto i = (s_NextSpeechSlot + n) % size;
        if (s_PedSpeechSlots[i].Status == CAEPedSpeechSlot::eStatus::FREE) {
            s_NextSpeechSlot = (uint16)((i + 1u) % size);
            return (int32)i;
        }
    }
    return -1;
}

uint32& CAEPedSpeechAudioEntity::GetNextPlayTimeRef(eGlobalSpeechContext gCtx) {
    return IsGlobalContextPain(gCtx)
        ? m_NextTimeCanSayPain[gCtx - CTX_GLOBAL_PAIN_START + 1]
        : gGlobalSpeechContextNextPlayTime[gCtx];
}

// notsa
const tGlobalSpeechContextInfo* CAEPedSpeechAudioEntity::GetGlobalSpeechContextInfo(eGlobalSpeechContext gCtx) {
    // Must use a loop because there are a few skipped values (TODO: Though I guess we could fix this?)
    for (const auto& e : gSpeechContextLookup) {
        if (e.GCtx == gCtx) {
            return &e;
        }
    }
    return nullptr;
}

// notsa
const tSpecificSpeechContextInfo* CAEPedSpeechAudioEntity::GetSpecificSpeechContextInfo(eSpecificSpeechContext sCtx, eGlobalSpeechContext gCtx, eAudioPedType pt, ePedSpeechVoiceS16 voice) {
    if (IsGlobalContextPain(gCtx)) {
        return &gPainSpeechLookup[sCtx][voice];
    }
    switch (pt) {
    case PED_TYPE_GEN:    return &gGenSpeechLookup[sCtx][voice];
    case PED_TYPE_EMG:    return &gEmgSpeechLookup[sCtx][voice];
    case PED_TYPE_PLAYER: return &gPlySpeechLookup[sCtx][voice];
    case PED_TYPE_GANG:   return &gGngSpeechLookup[sCtx][voice];
    case PED_TYPE_GFD:    return &gGfdSpeechLookup[sCtx][voice];
    default:              return nullptr;
    }
}

eSoundBank CAEPedSpeechAudioEntity::GetVoiceSoundBank(eGlobalSpeechContext gCtx, eAudioPedType pt, ePedSpeechVoiceS16 voice) {
    if (IsGlobalContextPain(gCtx)) {
        return gPainVoiceToBankLookup[voice];
    }
    switch (pt) {
    case PED_TYPE_GEN:    return gGenSpeechVoiceToBankLookup[voice];
    case PED_TYPE_EMG:    return gEmgSpeechVoiceToBankLookup[voice];
    case PED_TYPE_PLAYER: return gPlySpeechVoiceToBankLookup[voice];
    case PED_TYPE_GANG:   return gGngSpeechVoiceToBankLookup[voice];
    case PED_TYPE_GFD:    return gGfdSpeechVoiceToBankLookup[voice];
    default:              NOTSA_UNREACHABLE();
    }
}
