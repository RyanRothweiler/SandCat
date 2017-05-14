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
				// Actions
				{
					int actionsCount = SandCat.instance.GetActionsCount();
					showActions = EditorGUILayout.Foldout(showActions, "Actions (" + actionsCount + ")");
					if (showActions) {
						for (int index = 0; index < actionsCount; index++) {
							EditorGUILayout.LabelField(SandCat.instance.GetActionIndexName(index));
						}
					}
				}

				// Methods
				{
					int methodsCount = SandCat.instance.GetMethodsCount();
					showMethods = EditorGUILayout.Foldout(showMethods, "Methods (" + methodsCount + ")");
					if (showMethods) {
						for (int index = 0; index < methodsCount; index++) {
							EditorGUILayout.LabelField(SandCat.instance.GetMethodIndexName(index));
						}
					}
				}
			}
		}
	}
}