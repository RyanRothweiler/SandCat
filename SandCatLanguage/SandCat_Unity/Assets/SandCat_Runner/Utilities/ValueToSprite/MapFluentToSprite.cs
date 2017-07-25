using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Image))]
public class MapFluentToSprite : MonoBehaviour
{

	public StateValue stateVal;

	public ValuesToSprite pairing;
	private Image image;

	void Start ()
	{
		image = this.GetComponent<Image>();
	}

	// Update is called once per frame
	void Update ()
	{
		int fluentValue = (int)stateVal.GetValue();

		foreach (ValuesToSprite.Pair pair in pairing.pairings) {
			if (fluentValue == pair.value) {
				image.sprite = pair.image;
			}
		}

		if (image.sprite == null) {
			image.enabled = false;
		} else {
			image.enabled = true;
		}
	}
}
