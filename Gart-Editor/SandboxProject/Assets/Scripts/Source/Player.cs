using System;

using Gart;

namespace Sandbox
{
    public class Player: Entity
    {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_rigidBody2D;
        public Vector3 translation;
        public float Speed = 0.0f;
        public float Time = 0.0f;
        public float offset = 0.5f;

        Entity m_Camera;
        TransformComponent m_cameraTransform;
        Vector3 m_CameraTranslation;

       
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreated - {ID}");
            Console.WriteLine($"Speed : {Speed}");
            m_Transform = GetComponent<TransformComponent>();
            m_rigidBody2D = GetComponent<RigidBody2DComponent>();
            if (m_rigidBody2D != null) Console.WriteLine("RigidBody Component Present");

            m_Camera = FindEntityByName("Main Camera");

            if(m_Camera != null)
            {
                Console.WriteLine("camera present");
                m_cameraTransform = m_Camera.GetComponent<TransformComponent>();
            }
            else
            {
                Console.WriteLine("camera not found");
            }
            
        }

        void OnUpdate(float ts)
        {
            Time += ts;
            Vector2 velocity = Vector2.Zero;
            
            translation = m_Transform.Translation;
            if(m_cameraTransform != null)
            {
                m_CameraTranslation = m_cameraTransform.Translation;
                m_CameraTranslation.xy = m_Transform.Translation.xy;
                m_cameraTransform.Translation = m_CameraTranslation;
            }
            

            if (Input.GetKeyDown(Keycode.Space)) Console.WriteLine("Working");

            if (Input.GetKeyDown(Keycode.W)) velocity.y += 1.0f;
            if (Input.GetKeyDown(Keycode.Left)) velocity.x -= 1.0f;
            if (Input.GetKeyDown(Keycode.Right)) velocity.x += 1.0f;

            velocity *= Speed * ts;

            m_rigidBody2D.ApplyImpulse(velocity, true);

        }
    }
}
