#ifndef SWTOR_IDAMAGEABLE_H
#define SWTOR_IDAMAGEABLE_H

class IDamageable
{
public:
    virtual void takeDamage(float damage) = 0;
    virtual void heal(float healValue) = 0;
    virtual float getMaxHealth() = 0;
    virtual float getHealth() = 0;
    virtual bool isAlive() = 0;
};

class HealthAttribute : IDamageable
{
    IDamageable* _damageable;
public:
    void addDamageable(IDamageable* damageable){
        _damageable = damageable;
    }
    
    void takeDamage(float damage) override{
        _damageable->takeDamage(calculateDamage(damage));
    }

    void heal(float healValue) override{
        _damageable->takeDamage(calculateHealValue(healValue));
    }

    float getMaxHealth() override{
        return _damageable->getMaxHealth();
    }

    float getHealth() override{
        return _damageable->getHealth();
    }

    bool isAlive() override{
        return _damageable->isAlive();
    }
protected:
    IDamageable* getNextAttribute(){
        return _damageable;
    }
private:
    virtual float calculateDamage(float damage){
        return damage;
    }
    virtual float calculateHealValue(float healValue){
        return healValue;
    }
};

class TemporaryHealthAttribute : public HealthAttribute{
private:
    float _duration;
    float _currentTime = 0;
    bool _isDestroyed = false;
    HealthAttribute* _previousAttribute;
public:
    TemporaryHealthAttribute(float duration) : HealthAttribute(), _duration(duration) {}

    void initialize(HealthAttribute* previousAttribute){
        _previousAttribute = previousAttribute;
    }

    void update(float deltaTime) {
        _currentTime += _duration;
        
        if(_currentTime > _duration){
            destroy();
        }
    }

    bool isDestroyed(){
        return _isDestroyed;
    }
private:
    void destroy(){
        *_previousAttribute = getNextAttribute();
    }
};

#endif //SWTOR_IDAMAGEABLE_H