using System;

using Gart;

namespace Sandbox
{
    public class Player: Entity
    {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_rigidBody2D;
        Vector3 translation;
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreated - {ID}");
            m_Transform = GetComponent<TransformComponent>();
            m_rigidBody2D = GetComponent<RigidBody2DComponent>();
            if (m_rigidBody2D != null) Console.WriteLine("RigidBody Component Present");
        }

        void OnUpdate(float ts)
        {
            float speed = 1.0f;

            Vector2 velocity = Vector2.Zero;

            translation = m_Transform.Translation;
            Console.WriteLine($"Player.OnUpdate: {ts}");

            /*if (Input.GetKeyDown(Keycode.A)) translation.x -= speed * ts;
            else if (Input.GetKeyDown(Keycode.D)) translation.x += speed * ts;
            else if (Input.GetKeyDown(Keycode.W)) translation.y += speed * ts;
            else if (Input.GetKeyDown(Keycode.S)) translation.y -= speed * ts;*/

            if (Input.GetKeyDown(Keycode.W)) velocity.y += 1.0f;
            if (Input.GetKeyDown(Keycode.A)) velocity.x -= 1.0f;
            if (Input.GetKeyDown(Keycode.D)) velocity.x += 1.0f;

            velocity *= speed;

            m_rigidBody2D.ApplyImpulse(velocity, true);


            /* m_Transform.Translation = translation;*/
        }
    }
}
