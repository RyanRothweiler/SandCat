using UnityEngine;
using UnityEditor;
[CustomEditor(typeof(SandCat))]
public class SandCatEditor : Editor
{

	public bool showDebugInfo;

	public bool showActions;
	public bool showMethods;

	public override void OnInspectorGUI()
	{
		DrawDefaultInspector();

		SandCat cat = (SandCat)target;
		if (GUILayout.Button("Reload Rules")) {
			cat.ReloadRules();
		}



		// showDebugInfo = EditorGUILayout.Foldout(showDebugInfo, "Show Debug Info");
		if (true && SandCat.instance != null) {

			if (cat.prevError != "x")  {
				EditorGUILayout.LabelField("Syntax Error : ", EditorStyles.boldLabel);
				EditorGUILayout.LabelField(cat.prevError);
			} else {
				// Events
				{
					int methodsCount = SandCat.instance.GetEventsCount();
					showMethods = EditorGUILayout.Foldout(showMethods, "PlayerActions / Events (" + methodsCount + ")");
					if (showMethods) {
						for (int index = 0; index < methodsCount; index++) {
							EditorGUILayout.LabelField(SandCat.instance.GetEventIndexName(index));
						}
					}
				}
			}
		}
	}
}