#ifndef UNNAMED_PROJECT_ANIMATOR_H
#define UNNAMED_PROJECT_ANIMATOR_H

#include <QObject>
#include <QString>
#include <QVector3D>

enum class AnimationType
{
    Position,
    Rotation,
    Scaling
};


enum class InterpolationType
{
    Jump,
    Linear,
    Trigonometric
};
// TODO: maybe allow user to define own interpolation function

class ObjectBase;
class GlobalState;

class Animator : public QObject
{
    Q_OBJECT
public:

    Animator(ObjectBase *object,
             GlobalState *state,
             const QString &attributeKey,
             InterpolationType interpolation = InterpolationType::Linear,
             float animationTime = 1.0f);

    virtual ~Animator();

    virtual AnimationType getAnimationType() = 0;

    ObjectBase *getObject();

    /**
     * @brief tick should be called by the game
     * @param currentTime current time in seconds
     */
    void tick(float currentTime);

    /**
     * @brief instantlyFinishCurrentAnimation
     *        should mainly be used internal, but can be used
     *        from the outside as something like a "red panic button"
     */
    void instantlyFinishCurrentAnimation();

public slots:

    /**
    * @brief onValueChanged the animator will change object's attribute to the
    *        given value (over the given time)
    * @param val the new value
    * @param currentTime
    */
    void onValueChanged(const QVector3D &val, float currentTime);

signals:

    void animationStarted(Animator *anim);
    void animationFinished(Animator *anim);

protected:

    virtual void updateObject() = 0;
    virtual const QVector3D &getObjectsValue() = 0;

    ObjectBase *m_object;
    GlobalState *m_state;

    bool m_isAnimationRunning;

    InterpolationType m_interpolationType;

    float m_animationStartTime;
    float m_animationEndTime;

    QVector3D m_startValue;
    QVector3D m_endValue;
    QVector3D m_interpolatedValue;

    float m_animationTime;
};

#endif // UNNAMED_PROJECT_ANIMATOR_H
