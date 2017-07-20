using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DiscHeld : MonoBehaviour
{

	public Disc discHeld;

	void Update ()
	{
		int discSize = (int)SandCat.instance.GetFluentValue("DiscHeld");
		discHeld.SetDisc(discSize - 1);

	}
}
