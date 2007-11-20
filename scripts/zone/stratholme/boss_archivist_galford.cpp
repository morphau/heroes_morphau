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

/* ScriptData
SDName: boss_archivist_galford
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****

//Status: check for timers

#define SPELL_FIRENOVA    23462
#define SPELL_BURNINGWINDS    17293
#define SPELL_PYROBLAST    17274

struct MANGOS_DLL_DECL boss_archivist_galfordAI : public ScriptedAI
{
    boss_archivist_galfordAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FireNova_Timer;
    uint32 BurningWinds_Timer;
    uint32 Pyroblast_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        FireNova_Timer = 17000;
        BurningWinds_Timer = 12000;
        Pyroblast_Timer = 3000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //FireNova
        if (FireNova_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FIRENOVA);

            //17 seconds until we should cast this again
            FireNova_Timer = 17000;
        }else FireNova_Timer -= diff;

        //BurningWinds
        if (BurningWinds_Timer < diff)
        {
            //Cast
            if (rand()%100 < 50) //50% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_BURNINGWINDS);
            }

            //15 seconds until we should cast this again
            BurningWinds_Timer = 15000;
        }else BurningWinds_Timer -= diff;

        //Pyroblast
        if (Pyroblast_Timer < diff)
        {
            //Cast
            if (rand()%100 < 55) //55% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_PYROBLAST);
            }
            //21 seconds until we should cast this again
            Pyroblast_Timer = 21000;
        }else Pyroblast_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_archivist_galford(Creature *_Creature)
{
    return new boss_archivist_galfordAI (_Creature);
}


void AddSC_boss_archivist_galford()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_archivist_galford";
    newscript->GetAI = GetAI_boss_archivist_galford;
    m_scripts[nrscripts++] = newscript;
}
