using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class APlayer : MonoBehaviour
{

	public Grid grid;

	void Update ()
	{
		// place player on grid
		{
			int x = (int)SandCat.instance.GetFluentValue("PlayerX");
			int y = (int)SandCat.instance.GetFluentValue("PlayerY");
			this.transform.position = grid.GridToWorldPos(new Vector2(x, y));
		}
	}
}
