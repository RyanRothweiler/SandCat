using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Disc : MonoBehaviour
{

	public GameObject[] discs;

	public void SetDisc(int disc)
	{
		for (int index = 0; index < discs.Length; index++) {
			discs[index].SetActive(false);
		}

		if (disc > -1) {
			discs[disc].SetActive(true);
		}
	}
}