using UnityEngine;
using UnityEditor;
[CustomEditor(typeof(SandCat))]
public class SandCatEditor : Editor
{
	public override void OnInspectorGUI()
	{
		DrawDefaultInspector();

		SandCat cat = (SandCat)target;
		if (GUILayout.Button("Reload Rules")) {
			cat.ReloadRules();
		}
	}
}