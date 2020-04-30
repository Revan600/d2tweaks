#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct linkage;

		struct playerclass_line;
		struct bodylocs_line;
		struct storepage_line;
		struct elemtypes_line;
		struct hitclass_line;
		struct monmode_line;
		struct properties_line;

		struct data_tables {
			playerclass_line* player_class;                                 //+00000000   01415B48      playerclass.txt
			linkage* player_class_info;                     //+00000004   01410524      playerclass info

			bodylocs_line* body_locs;                                    //+00000008   01415578      bodylocs.txt
			linkage* body_locs_info;                        //+0000000C   01410504      bodylocs info

			storepage_line* store_page;                                   //+00000010   01414CC8      storepage.txt
			linkage* store_page_info;                       //+00000014   01415B24      storepage info

			elemtypes_line* elemtypes;                                   //+00000018   01414958      elemtypes.txt
			linkage* elemtypes_info;                       //+0000001C   01415B04      elemtypes info

			hitclass_line* hit_class;                                    //+00000020   01414148      hitclass.txt
			linkage* hit_class_info;                        //+00000024   01415AE4      hitclass info

			monmode_line* mon_mode;                                 //+00000028   01416878      monmode.txt
			linkage* mon_mode_info;                         //+0000002C   01415514      monmode info

			void* pPlayerModeStub;                              //+00000030   014162E8      plrmode.txt
			linkage* iPlayerMode;                      //+00000034   01414934      plrmode info
			void* pSkillCalc;                                   //+00000038   05279860      skillcalc.txt
			linkage* iSkillCalc;                       //+0000003C   01410014      skillcalc info
			uint8_t* pSkillsCode;                                  //+00000040   0537A514      skillscode formulae
			uint32_t dwSkillsCodeSize;                             //+00000044   00001768      size of skillscode block
			uint32_t dwSkillsCodeSizeEx;                           //+00000048   00001768      size of skillscode block
			uint8_t* pSkillDescCode;                               //+0000004C   0535C994      skilldesccode formulae
			uint32_t dwSkillDescCodeSize;                          //+00000050   0000107F      size of skilldesccode block
			uint32_t dwSkillDescCodeSizeEx;                        //+00000054   0000107F      size of skilldesccode block
			const char* pMissileCalc;                           //+00000058   01419C28      misscalc.txt
			linkage* iMissileCalc;                     //+0000005C   01417024      misscalc info
			uint8_t* pMissCode;                                    //+00000060   014A4944      misscode formulae
			uint32_t dwMissCodeSize;                               //+00000064   000000D4      size of misscode block
			uint32_t dwMissCodeSizeEx;                             //+00000068   000000D4      size of misscode block
			const char* pSkillCode;                             //+0000006C   052C445C      skillcode.txt (Id from skills.txt)
			linkage* iSkillCode;                       //+00000070   0141F084      skillcode info
			void* pEvents;                                      //+00000074   0141ACA8      events.txt
			linkage* iEvents;                          //+00000078   0141F064      events info
			uint32_t* pCompCodes;                                  //+0000007C   06C4FAFC      compcode.txt
			linkage* iCompCodes;                       //+00000080   01410544      compcode info
			int nCompCodes;                                     //+00000084   00000073      # of compcode records
			void* pMonAI;                                       //+00000088   0564351C      monai.txt
			linkage* iMonAI;                           //+0000008C   01414914      monai info
			int nMonAI;                                         //+00000090   00000098      # of monai records
			linkage* iItems;                           //+00000094   014BA014      items info
			uint8_t* pItemsCode;                                   //+00000098   013FDED8      itemscode formulae
			uint32_t dwItemsCodeSize;                              //+0000009C   000010E0      size of itemscode block
			uint32_t dwItemsCodeSizeEx;                            //+000000A0   000010E0      size of itemscode block
			//properties* pProperties;                   //+000000A4   0579E218      properties.txt
			//linkage* iProperties;                      //+000000A8   01489464      properties info
			//int nProperties;                                    //+000000AC   00000110      # of properties records
			//linkage* iRunes;                           //+000000B0   014C4774      runes info
			//MercDescTXT* pMercDesc;                             //+000000B4   01417208      hiredesc.txt
			//linkage* iMercDesc;                        //+000000B8   01415534      hiredesc info
			//FileStatesTable* pStates;                           //+000000BC   05767EA0      states.txt (limit = 255)
			//linkage* iStates;                          //+000000C0   014A16C4      states info
			//int nStates;                                        //+000000C4   000000BD      # of states records
			//void* pStateMasks;                                  //+000000C8   014A2574      statemasks.txt
			//uint32_t* fStateMasks[40];                             //+000000CC   014A2574      statemasks[40] (statemasks.txt segment array)
			//short* pProgressiveStates;                          //+0000016C   014A23E4      progressive-state list
			//int nProgressiveStates;                             //+00000170   00000006      # of progressive states
			//short* pCurseStates;                                //+00000174   014A2254      curse-state list
			//int nCurseStates;                                   //+00000178   0000001A      # of curse states
			//short* pTransformStates;                            //+0000017C   014A20C4      transform-state list
			//int nTransformStates;                               //+00000180   00000007      # of transform states
			//short* pActionStates;                               //+00000184   014A3E74      action-state list
			//int nActionStates;                                  //+00000188   00000007      # of action states
			//short* pColourStates;                               //+0000018C   014A3CE4      color-state list
			//int nColourStates;                                  //+00000190   00000002      # of color states
			//void* pSoundCodes;                                  //+00000194   057656BC      soundcode.txt (sound column from sounds.txt)
			//linkage* iSoundCodes;                      //+00000198   0141F0E4      soundcode info
			//int nSoundCodes;                                    //+0000019C   00000000      # of soundcode records (blanked out later)
			//FileHirelingsTable* pHirelings;                     //+000001A0   055D8CD8      hirelings.txt (limit = 256)
			//int nHirelings;                                     //+000001A4   00000078      # of hirelings records
			//int pMercFirst[256];                                //+000001A8   00000000      array of 256 integers (namefirst column from hirelings.txt)
			//int pMercLast[256];		                            //+000005A8   0000000C      array of 256 integers (namelast column from hirelings.txt)   
			//void* pNPCs;                                        //+000009A8   05724F74      npcs.txt
			//int nNPCs;                                          //+000009AC   00000011      # of npcs records
			//void* pColours;                                     //+000009B0   01417568      colors.txt
			//linkage* iColours;                         //+000009B4   01415554      colors info
			//linkage* iTreasureClassEx;                 //+000009B8   014C4714      treasureclassex info
			//D2TCExShortStrc* pTreasureClassEx;                  //+000009BC   05718D98      treasureclassex.txt (limit = 65534 - autotcs#)
			//int nTreasureClassEx;                               //+000009C0   0000043C      # of treasureclassex records
			//uint32_t* aTreasureClass[45];                          //+000009C4   0571D074      chest treasureclassex list (an array of 45 pointers)
			//FileMonstatsTable* pMonstats;                       //+00000A78   04FE0014      monstats.txt (limit = 32766)
			//linkage* iMonstats;                        //+00000A7C   0143C024      monstats info
			//int nMonstats;                                      //+00000A80   000002E1      # of monstats records
			//void* pMonSounds;                                   //+00000A84   013EBC9C      monsounds.txt
			//linkage* iMonSounds;                       //+00000A88   01438024      monsounds info
			//int nMonSounds;                                     //+00000A8C   0000008D      # of monsounds records
			//FileMonstats2Table* pMonstats2;                     //+00000A90   05287408      monstats2.txt (no sign of that 1023 record limit)
			//linkage* iMonstats2;                       //+00000A94   01502014      monstats2 info
			//int nMonstats2;                                     //+00000A98   00000245      # of monstats2 records
			//void* pMonPlace;                                    //+00000A9C   01412648      monplace.txt
			//linkage* iMonPlace;                        //+00000AA0   01417BA4      monplace info
			//int nMonPlace;                                      //+00000AA4   00000025      # of monplace records
			//void* pMonPreset;                                   //+00000AA8   057248B0      monpreset.txt
			//void* aMonPresetI[5];                               //+00000AAC   057248B0      array of 5 pointers to the monpreset sections for each of the 5 acts
			//void* aMonPresetII[5];                              //+00000AC0   0000002F      array of 5 integers (# of records for monpreset in each of the 5 acts)
			//FileSuperuniquesTable* pSuperUniques;               //+00000AD4   05364928      superuniques.txt (limit = 512)
			//linkage* iSuperUniques;                    //+00000AD8   0145A1F4      superuniques info
			//int nSuperUniques;                                  //+00000ADC   00000042      # of superunique records
			//uint16_t aSuperUniques[66];                             //+00000AE0   00010000      array of 66 uint16_ts (the IdNos of the default superuniques, new ones are not added here [thankfully])
			//FileMissilesTable* pMissiles;                       //+00000B64   05590014      missiles.txt (does some fixing for collidetype > 8)
			//linkage* iMissiles;                        //+00000B68   01492014      missiles info
			//int nMissiles;                                      //+00000B6C   000002C4      # of missiles records
			//FileMonlvlTable* pMonLvl;                           //+00000B70   013B0064      monlvl.txt
			//int nMonLvl;                                        //+00000B74   0000006F      # of monlvl records
			//void* pMonSeq;                                      //+00000B78   05375900      monseq.txt
			//linkage* iMonSeq;                          //+00000B7C   0143A024      monseq info
			//int nMonSeq;                                        //+00000B80   000003F2      # of monseq records
			//D2SeqRecordStrc* pMonSeqTable;                      //+00000B84   0143E7E4      sequences table (created from monseq.txt)
			//int nMonSeqTable;                                   //+00000B88   0000003C      # of sequences
			//FileSkilldescTable* pSkillDesc;                     //+00000B8C   05741104      skilldesc.txt (limit = 32766) [JL and not JLE]
			//linkage* iSkillDesc;                       //+00000B90   014B8024      skilldesc info
			//int nSkillDesc;                                     //+00000B94   000000DD      # of skilldesc records
			//FileSkillsTable* pSkills;                           //+00000B98   056E4D78      skills.txt (limit = 32766) [JL and not JLE]
			//linkage* iSkills;                          //+00000B9C   014B50E4      skills info
			//int nSkills;                                        //+00000BA0   00000173      # of skills records
			//int* nClassSkillCount;                              //+00000BA4   014B9014      class skill count list
			//int nHighestClassSkillCount;                        //+00000BA8   0000001E      # highest class skill count
			//short* nClassSkillList;                             //+00000BAC   014BCB54      class skill list
			//int nPassiveSkills;                                 //+00000BB0   0000001C      # of passive skills
			//uint16_t* pPassiveSkills;								//+00000BB4   014BCB04      passiveskill list
			//linkage* iOverlay;                         //+00000BB8   01484024      overlay info
			//FileOverlayTable* pOverlay;                         //+00000BBC   05352F54      overlay.txt
			//int nOverlay;                                       //+00000BC0   00000125      # of overlay records
			//FileCharstatsTable* pCharStats;                     //+00000BC4   057AD178      charstats.txt
			//int nCharStats;                                     //+00000BC8   00000007      # of charstats records
			//FileItemstatcostTable* pItemStatCost;               //+00000BCC   05219760      itemstatcost.txt (limit = 510) [511 used as END_OF_STATS in 'GF/JM' files]
			//linkage* iItemStatCost;                    //+00000BD0   0148C024      itemstatcost info
			//int nItemStatCost;                                  //+00000BD4   0000016E      # of itemstatcost records
			//void* pOPStats;										//+00000BD8   014882A4      opstats nesting table
			//int nOPStats;										//+00000BDC   000000D3      # of nested opstats
			//FileMonequipTable* pMonEquip;                       //+00000BE0   013B3798      monequip.txt (limit = 32766)
			//int nMonEquip;                                      //+00000BE4   0000002D      # of monequip records
			//FilePettypeTable* pPetType;                         //+00000BE8   05774138      pettype.txt (limit = 511)
			//linkage* iPetType;                         //+00000BEC   01486024      pettype info
			//int nPetType;                                       //+00000BF0   00000014      # of pettype records
			//linkage* iItemTypes;                       //+00000BF4   0141E024      itemtypes info
			//FileItemtypesTable* pItemTypes;                     //+00000BF8   050D14AC      itemtypes.txt
			//int nItemTypes;                                     //+00000BFC   00000069      # of itemtypes records
			//int nItemTypesIndex;                                //+00000C00   00000004      (itemtypes#+31)/32
			//uint32_t* pItemTypesNest;                              //+00000C04   0537C41C      itemtypes nesting table
			//linkage* iSets;                            //+00000C08   014B3CE4      sets info
			//void* pSets;                                        //+00000C0C   057A162C      sets.txt (limit = 32766)
			//int nSets;                                          //+00000C10   00000020      # of sets records
			//linkage* iSetItems;                        //+00000C14   014B1024      setitems info
			//FileSetitemsTable* pSetItems;                       //+00000C18   056BBAC0      setitems.txt (limit = 32766)
			//int nSetItems;                                      //+00000C1C   0000007F      # of setitems records
			//linkage* iUniqueItems;                     //+00000C20   014AA044      uniqueitems info
			//FileUniqueitemsTable* pUniqueItems;                 //+00000C24   0510E8B4      uniqueitems.txt (limit = 32766)
			//int nUniqueItems;                                   //+00000C28   00000191      # of uniqueitems records
			//linkage* iMonProp;                         //+00000C2C   01439024      monprop info
			//FileMonpropTable* pMonProp;                         //+00000C30   05132A2C      monprop.txt
			//int nMonProp;                                       //+00000C34   0000000E      # of monprop records
			//linkage* iMonType;                         //+00000C38   0141C024      montype info
			//void* pMonType;                                     //+00000C3C   06C4F014      montype.txt
			//int nMonType;                                       //+00000C40   0000003B      # of montype records
			//uint32_t* pMonTypeNest;                                //+00000C44   0141AAB4      montype nesting table
			//int nMonTypeIndex;                                  //+00000C48   00000002      (montype#+31)/32
			//linkage* iMonUMod;                         //+00000C4C   0145A274      monumod info
			//FileMonumodTable* pMonUMod;                         //+00000C50   0654F014      monumod.txt (limit = 512)
			//int nMonUMod;                                       //+00000C54   0000002B      # of monumod records
			//FileLevelsTable* pLevels;                           //+00000C58   0562D0EC      levels.txt (limit = 1023)
			//int nLevels;                                        //+00000C5C   00000096      # of levels records
			//FileLevelDefsTable* pLevelDefs;                     //+00000C60   055E134C      leveldefs.txt
			//FileLevelPresetTable* pLevelPreset;                 //+00000C64   0502C7E8      lvlprest.txt
			//int nLevelPreset;                                   //+00000C68   00000447      # of lvlprest records
			//int nStuff;                                         //+00000C6C   00000006      stuff value (ItemStatCost.txt + 0x140, record #1)
			//int cStuff;                                         //+00000C70   0000003F      2 ^ stuff - 1 (used as a controller field for opstats and other special stats)
			//FileAnimDataTable* pAnimData;                       //+00000C74   052369C0      sgptAnimTables (see below)
			//FileExperienceTable* pExperience;                   //+00000C78   05ECF014      experience.txt
			//FileDifficultyLevelsTable* pDifficultyLevels;       //+00000C7C   05750CD8      difficultylevels.txt (recordNo must equal 3)
			//int nDifficultyLevels;                              //+00000C80   00000003      # of difficultylevels records
			//BOOL bCompileTXT;                                   //+00000C84                 -txt switch
			//void* ExpFieldI[6];									//+00000C88
			//uint32_t unk0[4];										//+00000CA0
			//void* pExpField;									//+00000CB0
			//void* ExpFieldII[4];								//+00000CB4
			//void* pCubeMain;									//+00000CC4
			//int nCubeMain;										//+00000CC8
			//int nInventory;										//+00000CCC
			//D2InventoryRecordStrc* pInventory;					//+00000CD0
			//uint32_t unk1;											//+00000CD4
			//int nMasterItemList;								//+00000CD8
			//FileItemTable* pMasterItemList;						//+00000CDC
			//void* pWeapons;										//+00000CE0
			//int nWeapons;										//+00000CE4
			//void* pArmor;										//+00000CE8
			//int nArmor;											//+00000CEC
			//void* pMisc;										//+00000CF0
			//int nMisc;											//+00000CF4
			//uint32_t unk2;											//+00000CF8
			//int nGems;											//+00000CFC
			//FileGemsTable* pGems;								//+00000D00
			//int nLowQualityItems;								//+00000D04
			//FileLowQualityItemsTable* pLowQualityItems;			//+00000D08
			//int nBooks;											//+00000D0C
			//FileBooksTable* pBooks;								//+00000D10
			//int nRareAffix;										//+00000D14
			//FileRareAffixTable* pRareAffix;						//+00000D18
			//FileRareAffixTable* pRareSuffix;					//+00000D1C
			//FileRareAffixTable* pRarePrefix;					//+00000D20
			//int nItemRatio;										//+00000D24
			//FileItemratioTable* pItemRatio;						//+00000D28
			//uint32_t unk3;											//+00000D2C
			//int nGamble;										//+00000D30
			//uint32_t* pGambleSelection;							//+00000D34
			//int* pGambleChooseLimit[100];						//+00000D3C
			//int nAffix;											//+00000EC8
			//FileAffixTable* pAffix;								//+00000ECC [suffixes][prefixes][automagic]
			//FileAffixTable* pMagicSuffix;						//+00000ED0
			//FileAffixTable* pMagicPrefix;						//+00000ED4
			//FileAffixTable* pAutoMagic;							//+00000ED8
			//int nRunes;											//+00000EDC
			//void* pRunes;										//+00000EE0
			//int nQualityItems;									//+00000EE4
			//FileQualityitemsTable* pQualityItems;				//+00000EE8
			//uint32_t unk4;											//+00000EEC
			//uint32_t dwHiSeed;										//+00000EF0
			//uint32_t dwLoSeed;										//+00000EF4
			//uint32_t dwAutoMapRand[72];							//+00000EFC
			//void* pLvlTypes;									//+00001018
			//int* pPortalLevels;									//+0000101C
			//int nPortalLevels;									//+00001020
			//int nLvlTypes;										//+00001024
			//FileLevelWarpTable* pLevelWarp;						//+00001028
			//int nLevelWarp;										//+0000102C
			//FileLevelMazeTable* pLevelMaze;						//+00001030
			//int nLevelMaze;										//+00001034
			//void* pLvlSub;										//+00001038
			//int nLvlSub;										//+0000103C
			//void* sLvlSub;										//+00001040
			//uint32_t unk5;											//+00001044
			//void* ppLvlTypes;									//+00001048
			//uint32_t unk6;											//+0000104C
			//D2AutoMapShortStrc* pAutoMap;						//+00001050
			//int nAutoMap;										//+00001054
			//void* pMonLink;										//+00001058
			//int nMonItemPercent;								//+0000105C
			//void* pMonItemPercent;								//+00001060
			//void* pUniqueTitle;									//+00001064
			//void* pUniquePrefix;								//+00001068
			//void* pUniqueSuffix;								//+0000106C
			//void* pUniqueAppelation;							//+00001070
			//int nUniqueTitle;									//+00001074
			//int nUniquePrefix;									//+00001078
			//int nUniqueSuffix;									//+0000107C
			//int nUniqueAppelation;								//+00001080
			//uint32_t unk7[4];										//+00001084
			//FileShrinesTable* pShrines;							//+00001094
			//int nShrines;										//+00001098
			//FileObjectsTable* pObjects;							//+0000109C
			//int nObjects;										//+000010A0
			//void* pObjGroup;									//+000010A4
			//int nObjectGroup;									//+000010A8
			//void* pArmType;										//+000010AC
			//int nMonMode;										//+000010B0
			//FileMonmodeTable* pMonMode[3];						//+000010B4
			//void* pMonLoc;										//+000010B8
			//int nObjTypeAndMode;								//+000010BC
			//void* pObjTypeAndMode;								//+000010C0
			//void* pObjType;										//+000010C4
			//void* pObjMode;										//+000010C8
			//int nPlayerTypeAndMode;								//+000010CC
			//void* pPlayerTypeAndMode;							//+000010D0
			//void* pPlayerType;									//+000010D4
			//void* pPlayerMode;									//+000010D8
		};
	}
}
