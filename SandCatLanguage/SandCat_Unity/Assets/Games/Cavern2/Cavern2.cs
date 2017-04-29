using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Cavern2 : MonoBehaviour
{
	public GameObject[] dwarvesHomeDisplay;

	// Use this for initialization
	void Start ()
	{

	}

	// Update is called once per frame
	void Update ()
	{
		// display dwarves which are available
		{
			for (int index = 0; index < dwarvesHomeDisplay.Length; index++) {
				dwarvesHomeDisplay[index].SetActive(false);
			}
			int dwarvesHomeCount = (int)SandCat.instance.GetFluentValue("PlayerDwarvesHome");
			for (int index = 0; index < dwarvesHomeCount; index++) {
				dwarvesHomeDisplay[index].SetActive(true);
			}
		}
	}
}
