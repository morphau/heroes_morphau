/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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


#include "config.h"
#include "ScriptMgr.h"
#include "../../game/TargetedMovementGenerator.h"

uint8 loglevel = 0;
int nrscripts;
UnitScript *m_scripts[MAX_SCRIPTS];

// -- Scripts to be added --

// Custom
extern void AddSC_marshal_mcbride();
extern void AddSC_skorn_whitecloud();

// Guards
extern void AddSC_guard_bluffwatcher();
extern void AddSC_guard_contested();
extern void AddSC_guard_darnassus();
extern void AddSC_guard_dunmorogh();
extern void AddSC_guard_durotar();
extern void AddSC_guard_elwynnforest();
extern void AddSC_guard_ironforge();
extern void AddSC_guard_mulgore();
extern void AddSC_guard_orgrimmar();
extern void AddSC_guard_stormwind();
extern void AddSC_guard_teldrassil();
extern void AddSC_guard_tirisfal();
extern void AddSC_guard_undercity();

// Others
extern void AddSC_battlemaster();
extern void AddSC_guildmaster();
extern void AddSC_travelmaster();

// Honor
extern void AddSC_Honor_Vendor();

// CreatureAI
extern void AddSC_kobold();
//extern void AddSC_default();    //testing 
extern void AddSC_default_caster(); //testing
extern void AddSC_generic_caster();

// -------------------

MANGOS_DLL_EXPORT
void ScriptsFree()
{                                                           // Free resources before library unload
    for(int i=0;i<nrscripts;i++)
        delete m_scripts[i];
}

MANGOS_DLL_EXPORT
void ScriptsInit()
{
    nrscripts = 0;
    for(int i=0;i<MAX_SCRIPTS;i++)
        m_scripts[i]=NULL;

    // -- Inicialize the Scripts to be Added --

    // Custom
    AddSC_marshal_mcbride();
    AddSC_skorn_whitecloud();

    // Guards
    AddSC_guard_bluffwatcher();
    AddSC_guard_contested();
    AddSC_guard_darnassus();
    AddSC_guard_dunmorogh();
    AddSC_guard_durotar();
    AddSC_guard_elwynnforest();
    AddSC_guard_ironforge();
    AddSC_guard_mulgore();
    AddSC_guard_orgrimmar();    
    AddSC_guard_stormwind();
    AddSC_guard_teldrassil();
    AddSC_guard_tirisfal();
    AddSC_guard_undercity();

    // Others
    AddSC_battlemaster();
    AddSC_guildmaster();
    AddSC_travelmaster();

    // Honor
    AddSC_Honor_Vendor();
    
    //CreatureAI
    AddSC_kobold();
  //  AddSC_default();
    AddSC_default_caster();
	AddSC_generic_caster();

    // ----------------------------------------

}

UnitScript* GetScriptByName(std::string Name)
{
    for(int i=0;i<MAX_SCRIPTS;i++)
    {
        if( m_scripts[i] && m_scripts[i]->Name == Name )
            return m_scripts[i];
    }
    return NULL;
}

MANGOS_DLL_EXPORT
bool GossipHello ( Player * player, Creature *_Creature )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipHello(player,_Creature);
}

MANGOS_DLL_EXPORT
bool GossipSelect( Player *player, Creature *_Creature,uint32 sender, uint32 action )
{
    UnitScript *tmpscript = NULL;

    printf("action: %d\n",action);

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelect(player,_Creature,sender,action);
}

MANGOS_DLL_EXPORT
bool GossipSelectWithCode( Player *player, Creature *_Creature, uint32 sender, uint32 action, char* sCode )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipSelectWithCode) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelectWithCode(player,_Creature,sender,action,sCode);
}

MANGOS_DLL_EXPORT
bool QuestAccept( Player *player, Creature *_Creature, Quest *_Quest )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestAccept(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestSelect( Player *player, Creature *_Creature, Quest *_Quest )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestSelect(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestComplete( Player *player, Creature *_Creature, Quest *_Quest )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestComplete) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestComplete(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool ChooseReward( Player *player, Creature *_Creature, Quest *_Quest, uint32 opt )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pChooseReward(player,_Creature,_Quest,opt);
}

MANGOS_DLL_EXPORT
uint32 NPCDialogStatus( Player *player, Creature *_Creature )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pNPCDialogStatus) return 100;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pNPCDialogStatus(player,_Creature);
}

MANGOS_DLL_EXPORT
bool ItemHello( Player *player, Item *_Item, Quest *_Quest )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemHello(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool ItemQuestAccept( Player *player, Item *_Item, Quest *_Quest )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemQuestAccept(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool GOHello( Player *player, GameObject *_GO )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOHello(player,_GO);
}

MANGOS_DLL_EXPORT
bool GOQuestAccept( Player *player, GameObject *_GO, Quest *_Quest )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOQuestAccept(player,_GO,_Quest);
}

MANGOS_DLL_EXPORT
bool GOChooseReward( Player *player, GameObject *_GO, Quest *_Quest, uint32 opt )
{
    UnitScript *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOChooseReward(player,_GO,_Quest,opt);
}

MANGOS_DLL_EXPORT
bool AreaTrigger      ( Player *player, Quest *_Quest, uint32 triggerID )
{
    UnitScript *tmpscript = NULL;

    // TODO: load a script name for the areatriggers
    //tmpscript = GetScriptByName();
    if(!tmpscript || !tmpscript->pAreaTrigger) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pAreaTrigger(player,_Quest,triggerID);
}

MANGOS_DLL_EXPORT
CreatureAI* GetAI(Creature *_Creature)
{
    UnitScript *tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);

    if(!tmpscript || !tmpscript->GetAI) return NULL;
    return tmpscript->GetAI(_Creature);
}

void ScriptedAI::UpdateAI(const uint32)
{
    if( m_creature->getVictim() != NULL )
    {
        if( needToStop() )
        {
            DoStopAttack();
        }
        else
        {
            if( m_creature->isAttackReady() )
            {
                if(!m_creature->canReachWithAttack(m_creature->getVictim()))
                    return;
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();

                if ( !m_creature->getVictim() )
                    return;

                if( needToStop() )
                    DoStopAttack();
            }
        }
    }
}

void ScriptedAI::AttackStop(Unit *)
{
    if( m_creature->isAlive() )
        DoGoHome();
}

void ScriptedAI::DoStartAttack(Unit* victim)
{
	if ( m_creature->Attack(victim) )
	{
		(*m_creature)->Mutate(new TargetedMovementGenerator(*victim));
	}
}

void ScriptedAI::DoStopAttack()
{
    if( m_creature->getVictim() != NULL )
    {
		m_creature->AttackStop();
    }
}

void ScriptedAI::DoGoHome()
{
    if( !m_creature->getVictim() && m_creature->isAlive() )
	{
        (*m_creature)->TargetedHome();
	}
}

bool ScriptedAI::needToStop() const
{
    return ( !m_creature->getVictim()->isTargetableForAttack() || !m_creature->isAlive() );
}

void ScriptedAI::DoPlaySoundToSet(Unit* unit, uint32 sound)
{
	WorldPacket data(4);
	data.SetOpcode(SMSG_PLAY_SOUND);
	data << uint32(sound);
	unit->SendMessageToSet(&data,false);
}