using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tower : MonoBehaviour
{

	public string entityName;
	public Disc[] discs;

	void Update ()
	{
		DoLevel("One", 0);
		DoLevel("Two", 1);
		DoLevel("Three", 2);
		DoLevel("Four", 3);
	}

	public void DoLevel(string strLevel, int intLevel)
	{
		int discSize = (int)SandCat.instance.GetEntityFluentValue(entityName, "Level" + strLevel);
		discs[intLevel].SetDisc(discSize - 1);
	}
}
