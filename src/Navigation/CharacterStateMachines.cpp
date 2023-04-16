#include "CharacterStateMachines.hpp"

/* Jedi */

void Jedi::setupStateMachine(NavMeshAgent* character, NavMeshAgent* target, Character* target_character, Animator* animator) {
    State* wanderingS = new WanderingState(character, {});
    State* followingS = new FollowState(character, target, {});
    State* attackS = new AttackState(animator, {});

    Transition* seeTargetT = new SeeTargetTransition(followingS, character, target, Fraction::Jedi);
    Transition* durationT = new AttackDurationTransition(followingS);
    Transition* distanceT = new AttackDistanceTransition(attackS, character, target);
    Transition* killT = new KillTransition(wanderingS, target_character);

    wanderingS->addTransition(seeTargetT);
    followingS->addTransition(distanceT);
    attackS->addTransition(durationT);
    attackS->addTransition(killT);

    _stateMachine = new StateMachine(wanderingS);
}

/* Squad Leader */

void SquadLeader::setupStateMachine(NavMeshAgent* character, NavMeshAgent* target, Character* target_character/*, Weapon* weapon*/) {
    State* patrollingS = new PatrollingState(character, {});
    State* followingS = new FollowState(character, target, {});
    State* shootS = new ShootingState(/*weapon, */{});

    Transition* seeTargetT = new SeeTargetTransition(followingS, character, target, Fraction::Soldier);
    Transition* durationT = new ShootingDurationTransition(followingS);
    Transition* distanceT = new ShootingDistanceTransition(shootS, character, target);
    Transition* killT = new KillTransition(patrollingS, target_character);

    patrollingS->addTransition(seeTargetT);
    followingS->addTransition(distanceT);
    shootS->addTransition(durationT);
    shootS->addTransition(killT);

    _stateMachine = new StateMachine(patrollingS);
}

/* Squad Member */

void SquadMember::setupStateMachine(NavMeshAgent* character, NavMeshAgent* target, NavMeshAgent* leader, Character* target_character/*, Weapon* weapon*/) {
    State* LfollowingS = new FollowState(character, leader, {});
    State* TfollowingS = new FollowState(character, target, {});
    State* shootS = new ShootingState(/*weapon, */{});
    State* wanderingS = new WanderingState(character, {});

    Transition* seeTargetT = new SeeTargetTransition(TfollowingS, character, target, Fraction::Soldier);
    Transition* durationT = new ShootingDurationTransition(TfollowingS);
    Transition* distanceT = new ShootingDistanceTransition(shootS, character, target);
    Transition* killT = new KillTransition(LfollowingS, target_character);
    Transition* LdiedT = new LeaderDiedTransition(wanderingS, sq_leader);

    LfollowingS->addTransition(seeTargetT);
    TfollowingS->addTransition(distanceT);
    shootS->addTransition(durationT);
    shootS->addTransition(killT);
    LfollowingS->addTransition(LdiedT);
    wanderingS->addTransition(seeTargetT);

    _stateMachine = new StateMachine(LfollowingS);
}
