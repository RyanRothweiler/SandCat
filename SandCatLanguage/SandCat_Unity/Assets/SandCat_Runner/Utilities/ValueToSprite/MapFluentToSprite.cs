using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Image))]
public class MapFluentToSprite : MonoBehaviour
{

	public FluentDisplayType type;


	public ValuesToSprite pairing;
	public string entityName;
	public string fluentName;

	private Image image;

	void Start ()
	{
		image = this.GetComponent<Image>();
	}

	// Update is called once per frame
	void Update ()
	{
		int fluentValue = 0;
		if (type == FluentDisplayType.Fluent) {
			fluentValue = (int)SandCat.instance.GetFluentValue(fluentName);
		} else if (type == FluentDisplayType.Entity) {
			fluentValue = (int)SandCat.instance.GetEntityFluentValue(entityName, fluentName);
		}

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
