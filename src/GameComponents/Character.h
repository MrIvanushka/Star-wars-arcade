class Character {
private:
    float _health;
    const float _maxHealth;

public:
    Character(float health): _health(health), _maxHealth(health) {}
    void TakeDamage(float damage);
    float GetHealth();
    void Heal(float health);
    bool IsDead();
};