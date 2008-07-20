/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Instance_Magisters_Terrace
SD%Complete: 100
SDComment:  Designed only for Selin Fireheart
SDCategory: Magister's Terrace
EndScriptData */

#include "def_magisters_terrace.h"
#include "GameObject.h"

#define NUMBER_OF_ENCOUNTERS      4
#define NUMBER_OF_CRYSTALS        5

/*
0  - Selin Fireheart
1  - Vexallus
2  - Priestess Delrissa
3  - Kael'thas Sunstrider
*/

struct MANGOS_DLL_DECL instance_magisters_terrace : public ScriptedInstance
{
    instance_magisters_terrace(Map* map) : ScriptedInstance(map)
    {
        Initialize();
    }

    uint32 Encounters[NUMBER_OF_ENCOUNTERS];
    uint64 FelCrystals[NUMBER_OF_CRYSTALS];
    uint32 FilledCrystals;
    uint64 SelinGUID;
	uint64 DelrissaGUID;
	uint64 VexallusDoorGUID;
	uint64 SelinDoorGUID;
    uint64 SelinEncounterDoorGUID;
	uint64 DelrissaDoorGUID;

    void Initialize()
    {
        for(uint8 i = 0; i < NUMBER_OF_ENCOUNTERS; i++)
            Encounters[i] = NOT_STARTED;
        for(uint8 i = 0; i < NUMBER_OF_CRYSTALS; i++)
            FelCrystals[i] = 0;

        FilledCrystals = 0;
        SelinGUID = 0;
		DelrissaGUID = 0;
        VexallusDoorGUID = 0;
		SelinDoorGUID = 0;
		SelinEncounterDoorGUID = 0;
		DelrissaDoorGUID = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < NUMBER_OF_ENCOUNTERS; i++)
            if(Encounters[i] == IN_PROGRESS)
                return true;
        return false;
    }

    uint32 GetData(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_SELIN_EVENT:
                return Encounters[0];

            case DATA_VEXALLUS_EVENT:
                return Encounters[1];

            case DATA_DELRISSA_EVENT:
                return Encounters[2];

            case DATA_KAELTHAS_EVENT:
                return Encounters[3];
        }
        return 0;
    }

    void SetData(uint32 identifier, uint32 data)
    {
        switch(identifier)
        {                
            case DATA_SELIN_EVENT:
                Encounters[0] = data;
                break;

            case DATA_VEXALLUS_EVENT:
                Encounters[1] = data;
                break;

            case DATA_DELRISSA_EVENT:
                Encounters[2] = data;
                break;

            case DATA_KAELTHAS_EVENT:
                Encounters[3] = data;
                break;
        }
    }

    void OnCreatureCreate(Creature *creature, uint32 entry)
    {
        switch(entry)
        {
            case 24723:
                SelinGUID = creature->GetGUID();
                break;
            case 24560:
                DelrissaGUID = creature->GetGUID();
                break;
            case 24722:
            {
                FelCrystals[FilledCrystals] = creature->GetGUID();
                ++FilledCrystals;
                break;
            }
        }
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_SELIN:
                return SelinGUID;
            case DATA_DELRISSA:
                return DelrissaGUID;
			case DATA_CRYSTAL_1:
                return FelCrystals[0];
            case DATA_CRYSTAL_2:
                return FelCrystals[1];
            case DATA_CRYSTAL_3:
                return FelCrystals[2];
            case DATA_CRYSTAL_4:
                return FelCrystals[3];
            case DATA_CRYSTAL_5:
                return FelCrystals[4];
            case DATA_VEXALLUS_DOOR:
                return VexallusDoorGUID;
            case DATA_SELIN_DOOR:
                return SelinDoorGUID;
            case DATA_SELIN_ENCOUNTER_DOOR:
                return SelinEncounterDoorGUID;
			case DATA_DELRISSA_DOOR:
                return DelrissaDoorGUID;
        }
        return 0;
    }

    void OnObjectCreate(GameObject* go)
    {
        switch(go->GetEntry())
        {
            case 187896:
                VexallusDoorGUID = go->GetGUID();
                break;
            case 187979:
                SelinDoorGUID = go->GetGUID();
                break;
            case 188118:
                SelinEncounterDoorGUID = go->GetGUID();
				break;
			case 187770:
                DelrissaDoorGUID = go->GetGUID();
                break;
		}
    }
};

InstanceData* GetInstanceData_instance_magisters_terrace(Map* map)
{
    return new instance_magisters_terrace(map);
}

void AddSC_instance_magisters_terrace()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "instance_magisters_terrace";
    newscript->GetInstanceData = GetInstanceData_instance_magisters_terrace;
    m_scripts[nrscripts++] = newscript;
}
