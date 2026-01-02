using System;
using Gart;

namespace Sandbox
{
    public class TextDemo:Entity
    {
        TextComponent m_TexComp;

        void OnCreate()
        {
            m_TexComp = GetComponent<TextComponent>();
            if(m_TexComp != null)
            {
                Console.WriteLine("Text Component is Present");
                m_TexComp.Text = "Hi this text is written through c# side of Game Engine";
            }
        }

        void OnUpdate(float ts)
        {

        }

    }
}
