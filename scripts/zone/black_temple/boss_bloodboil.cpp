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
SDName: Boss_Bloodboil
SD%Complete: 80
SDComment: Bloodboil not working correctly
SDCategory: Black Temple
EndScriptData */

#include "../../sc_defines.h"
#include "def_black_temple.h"
#include "../../../../../game/TargetedMovementGenerator.h"
#include "../../../../../game/SpellAuras.h"

//Spells
#define SPELL_ACID_GEYSER        40630
#define SPELL_ACIDIC_WOUND       40481
#define SPELL_ARCING_SMASH       40599
#define SPELL_BLOODBOIL          42005 // This spell is AoE whereas it shouldn't
#define SPELL_FEL_ACID           40508
#define SPELL_FEL_RAGE_SELF      40594
#define SPELL_FEL_RAGE_TARGET    40604
#define SPELL_FEL_RAGE_2         40616
#define SPELL_FEL_RAGE_3         41625
#define SPELL_KNOCKBACK          38576
#define SPELL_TAUNT_GURTOGG      40603
#define SPELL_INSIGNIFIGANCE     40618

//Speech'n'Sound
#define SAY_AGGRO            "Horde will crush you!"
#define SOUND_AGGRO          11432

#define SAY_SLAY1            "Time to feast!"
#define SOUND_SLAY1          11433

#define SAY_SLAY2            "More! I want more!"
#define SOUND_SLAY2          11434

#define SAY_SPECIAL1         "Drink your blood! Eat your flesh!"
#define SOUND_SPECIAL1       11435

#define SAY_SPECIAL2         "I hunger!"
#define SOUND_SPECIAL2       11436

#define SAY_ENRAGE           "I'll rip the meat from your bones!"
#define SOUND_ENRAGE         11437

#define SOUND_DEATH          11439

//This is used to sort the players by distance in preparation for the Bloodboil cast.
struct TargetDistanceOrder : public std::binary_function<const Unit, const Unit, bool>
{
    const Unit* MainTarget;
    TargetDistanceOrder(const Unit* Target) : MainTarget(Target) {};
    // functor for operator ">"
    bool operator()(const Unit* _Left, const Unit* _Right) const
    {
        return (MainTarget->GetDistanceSq(_Left) > MainTarget->GetDistanceSq(_Right));
    }
};

struct MANGOS_DLL_DECL boss_gurtogg_bloodboilAI : public ScriptedAI
{
    boss_gurtogg_bloodboilAI(Creature *c) : ScriptedAI(c) 
     {
         pInstance = ((ScriptedInstance*)c->GetInstanceData());
         Reset();
     }

     ScriptedInstance* pInstance;
     
     uint64 TargetGUID;

     float TargetThreat;

     uint32 BloodboilTimer;
     uint32 BloodboilCount;
    uint32 AcidGeyserTimer;
    uint32 AcidicWoundTimer;
    uint32 ArcingSmashTimer;
    uint32 EnrageTimer;
    uint32 FelAcidTimer;
    uint32 KnockbackTimer;
    uint32 PhaseChangeTimer;

    bool Phase1;
    bool InCombat;

    void Reset()
    {
          if(pInstance)
               pInstance->SetData(DATA_GURTOGGBLOODBOILEVENT, 0);

          TargetGUID = 0;

          TargetThreat = 0;

          BloodboilTimer = 10000;
          BloodboilCount = 0;
          AcidGeyserTimer = 1000;
          AcidicWoundTimer = 6000;
          ArcingSmashTimer = 19000;
          EnrageTimer = 600000;
          FelAcidTimer = 25000;
          KnockbackTimer = 10000;
          PhaseChangeTimer = 60000;

          Phase1 = true;

          InCombat = false;

         (*m_creature).GetMotionMaster()->Clear(false);
     }

    void AttackStart(Unit *who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                Reset();
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
                    if(pInstance) pInstance->SetData(DATA_GURTOGGBLOODBOILEVENT, 1);
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {

        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(who && who->isAlive())
                    m_creature->AddThreat(who, 1.0f);

                if(!InCombat)
                {
                    DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                         if(pInstance) pInstance->SetData(DATA_GURTOGGBLOODBOILEVENT, 1);
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
            case 0:
                DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY1);
                break;
            case 1:
                DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY2);
                break;
        }
    }

    void JustDied(Unit *victim)
    {
        if(pInstance)
            pInstance->SetData(DATA_GURTOGGBLOODBOILEVENT, 3);

        InCombat = false;
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }

    // Note: This seems like a very complicated fix.
    void CastBloodboil()
    {
        // Get the Threat List
        std::list<HostilReference *> m_threatlist = m_creature->getThreatManager().getThreatList();
        
        if(!m_threatlist.size()) return; // He doesn't have anyone in his threatlist, useless to continue

        std::list<Unit *> targets;
        std::list<HostilReference *>::iterator itr = m_threatlist.begin();
        std::advance(itr, 0);
        for( ; itr!= m_threatlist.end(); ++itr) //store the threat list in a different container
        {
            Unit *target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
            if(target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER ) //only on alive players
                targets.push_back( target);
        }

        //Sort the list of players
        targets.sort(TargetDistanceOrder(m_creature));
        //Resize so we only get top 5
        targets.resize(5);
         
        //Aura each player in the targets list with Bloodboil. Aura code copied+pasted from Aura command in Level3.cpp
        /*SpellEntry const *spellInfo = GetSpellStore()->LookupEntry( SPELL_BLOODBOIL );   
        if(spellInfo)
        {
            for(std::list<Unit *>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                Unit* target = *itr;
                if(!target) return;
                for(uint32 i = 0;i<3;i++)
                {
                    uint8 eff = spellInfo->Effect[i];
                    if (eff>=TOTAL_SPELL_EFFECTS)
                        continue;
                    
                    Aura *Aur = new Aura(spellInfo, i, NULL, target);
                    target->AddAura(Aur);
                }
            }
        }*/
    }
     
    void RevertThreatOnTarget(uint64 guid)
    {
        Unit* pUnit = NULL;
        pUnit = Unit::GetUnit((*m_creature), guid);
        if(pUnit)
        {
            m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);               
            m_creature->AddThreat(pUnit, TargetThreat);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(ArcingSmashTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCING_SMASH);
            ArcingSmashTimer = 19000;
        }else ArcingSmashTimer -= diff;

        if(FelAcidTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FEL_ACID);
            FelAcidTimer = 25000;
        }else FelAcidTimer -= diff;

        if(EnrageTimer < diff)
        {
            //TODO: Find Enrage Spell
            EnrageTimer = 600000;
            DoYell(SAY_ENRAGE,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
        }else EnrageTimer -= diff;

        if(Phase1)
        {            
            if(KnockbackTimer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                if(target)
                {
                    (m_creature->getThreatManager()).modifyThreatPercent(target, -40);
                    DoCast(target, SPELL_KNOCKBACK);
                }
                KnockbackTimer = 22000;
            }else KnockbackTimer -= diff;

            if(AcidicWoundTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_ACIDIC_WOUND);
                AcidicWoundTimer = 10000;
            }else AcidicWoundTimer -= diff;
        }

        if(!Phase1)
        {
            if(AcidGeyserTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_ACID_GEYSER);
                AcidGeyserTimer = 30000;
            }else AcidGeyserTimer -= diff;
            
            if(BloodboilTimer < diff)
            {
                CastBloodboil();
                BloodboilCount++;
                if(BloodboilCount < 5)
                    BloodboilTimer = 10000*BloodboilCount;
                else return; // Only cast it five times
            }else BloodboilTimer -= diff;
        }

        if(PhaseChangeTimer < diff)
        {
            if(Phase1)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0); 
                if(target && target->isAlive())
                {                          
                    TargetThreat = m_creature->getThreatManager().getThreat(target);
                    TargetGUID = target->GetGUID();
                    target->CastSpell(m_creature, SPELL_TAUNT_GURTOGG, true);
                    m_creature->TauntApply(target);
                    // m_creature->AddThreat(target, 50000000.0f); // Add a lot of threat since TauntApply does not work properly.
                    DoCast(target, SPELL_FEL_RAGE_TARGET, true);
                    DoCast(m_creature, SPELL_INSIGNIFIGANCE, true); // If VMaps is disabled, this spell can call the whole instance
                    /* These spells do not work, comment them out for now.
                    DoCast(target, SPELL_FEL_RAGE_2, true);
                    DoCast(target, SPELL_FEL_RAGE_3, true);*/

                    //Cast this without triggered so that it appears in combat logs and shows visual.
                    DoCast(m_creature, SPELL_FEL_RAGE_SELF);
                    

                    switch(rand()%2)
                    {
                        case 0:
                            DoYell(SAY_SPECIAL1,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature, SOUND_SPECIAL1);
                            break;
                        case 1:
                            DoYell(SAY_SPECIAL2,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature, SOUND_SPECIAL2);
                            break;
                    }
                    AcidGeyserTimer = 1000;
                    Phase1 = false;
                    PhaseChangeTimer = 30000;
                }
            }

            if(!Phase1) // Encounter is a loop pretty much. Phase 1 -> Phase 2 -> Phase 1 -> Phase 2 till death or enrage
            {
                m_creature->TauntFadeOut(m_creature->getVictim());
                
                //if(TargetGUID)
                    //RevertThreatOnTarget(TargetGUID);
                Phase1 = true;
                BloodboilTimer = 10000;
                BloodboilCount = 0;
                AcidicWoundTimer += 2000;
                ArcingSmashTimer += 2000;
                FelAcidTimer += 2000;
                KnockbackTimer += 2000;
                PhaseChangeTimer = 60000;
            }
        }else PhaseChangeTimer -= diff;
           
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gurtogg_bloodboil(Creature *_Creature)
{
    return new boss_gurtogg_bloodboilAI (_Creature);
}

void AddSC_boss_gurtogg_bloodboil()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gurtogg_bloodboil";
    newscript->GetAI = GetAI_boss_gurtogg_bloodboil;
    m_scripts[nrscripts++] = newscript;
}
