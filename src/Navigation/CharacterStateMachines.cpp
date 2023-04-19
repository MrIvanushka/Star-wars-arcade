#include "CharacterStateMachines.hpp"

/* Jedi */

void JediAI::setupStateMachine(NavMeshAgent* character, std::vector<glm::vec3> wanderingTargets) {
    State* wanderingState = new WanderingState(character, wanderingTargets, {});
    //State* followingS = new FollowState(character, target, {});
   // State* attackS = new AttackState();

    //Transition* seeTargetT = new SeeTargetTransition(followingS, character, target, Fraction::Jedi);
    //Transition* durationT = new AttackDurationTransition(followingS);
    //Transition* distanceT = new AttackDistanceTransition(attackS, character, target);
    //Transition* killT = new KillTransition(wanderingS, target->getComponent<IDamageable>());

    //wanderingS->addTransition(seeTargetT);
    //followingS->addTransition(distanceT);
    //attackS->addTransition(durationT);
    //attackS->addTransition(killT);

    _stateMachine = new StateMachine(wanderingState);
}
/*

void SquadLeader::setupStateMachine(NavMeshAgent* character, GameObject* target) {
    State* patrollingS = new PatrollingState(character, {});
    State* followingS = new FollowState(character, target, {});
    State* shootS = new ShootingState({});

    Transition* seeTargetT = new SeeTargetTransition(followingS, character, target, Fraction::Jedi);
    Transition* durationT = new ShootingDurationTransition(followingS);
    Transition* distanceT = new ShootingDistanceTransition(shootS, character, target);
    Transition* killT = new KillTransition(patrollingS, target->getComponent<IDamageable>());

    patrollingS->addTransition(seeTargetT);
    followingS->addTransition(distanceT);
    shootS->addTransition(durationT);
    shootS->addTransition(killT);

    _stateMachine = new StateMachine(patrollingS);
}


void SquadMember::setupStateMachine(NavMeshAgent* character, GameObject* target) {
    State* LfollowingS = new FollowState(character, sq_leader, {});
    State* TfollowingS = new FollowState(character, target, {});
    State* shootS = new ShootingState({});
    State* wanderingS = new WanderingState(character, {});

    Transition* seeTargetT = new SeeTargetTransition(TfollowingS, character, target, Fraction::Jedi);
    Transition* durationT = new ShootingDurationTransition(TfollowingS);
    Transition* distanceT = new ShootingDistanceTransition(shootS, character, target);
    Transition* killT = new KillTransition(LfollowingS, target->getComponent<IDamageable>());
    Transition* LdiedT = new LeaderDiedTransition(wanderingS, sq_leader->getComponent<IDamageable>());

    LfollowingS->addTransition(seeTargetT);
    TfollowingS->addTransition(distanceT);
    shootS->addTransition(durationT);
    shootS->addTransition(killT);
    LfollowingS->addTransition(LdiedT);
    wanderingS->addTransition(seeTargetT);

    _stateMachine = new StateMachine(LfollowingS);
}
*/