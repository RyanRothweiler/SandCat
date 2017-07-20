using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResourceCell : MonoBehaviour
{

	public int arrayIndex;
	public GameObject[] resourceImages;
	public Grid board;

	// Use this for initialization
	void Start ()
	{

	}

	// Update is called once per frame
	void Update ()
	{
		// Resource display
		{
			for (int index = 0; index < resourceImages.Length; index++) {
				resourceImages[index].SetActive(false);
			}

			int res = (int)SandCat.instance.GetFluentInArray("HarvestSpace", arrayIndex, "Resource");
			resourceImages[res].SetActive(true);
		}

		// Position
		{
			int x = (int)SandCat.instance.GetFluentInArray("HarvestSpace", arrayIndex, "X");
			int y = (int)SandCat.instance.GetFluentInArray("HarvestSpace", arrayIndex, "Y");
			this.transform.position = board.GridToWorldPos(new Vector2(x, y));
		}
	}
}
